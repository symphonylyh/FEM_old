close all;
PLOT = true;
BOUNDARY_ENHANCE = true; % enhance boundary information

% img = imread('img0006_1.JPG');
img = imread('ball.jpg');


[h,w,d] = size(img);
sigma = floor(max(h,w) / 500); 
rgb = imguidedfilter(img, 'NeighborhoodSize', 2 * sigma + 1); 

% Convert to CIE Lab space
[L,a,b] = rgb2lab(rgb);

% Scale to [0,1]
L = mat2gray(L);
a = mat2gray(a);
b = mat2gray(b);

if PLOT
fig = 1;
figure(fig); fig = fig + 1;
[ha, pos] = tight_subplot(1,4,[.01 .01],[.01 .01],[.01 .01]);
axes(ha(1)), imshow(img), title('Original Image');
axes(ha(2)), imshow(L), title('L Channel');
axes(ha(3)), imshow(a), title('a Channel');
axes(ha(4)), imshow(b), title('b Channel');
end

% Calculate Euclidean distance from the average "tone", this step segments
% the object from the background
[Acounts, Avalues] = imhist(a);
Adistribution = cumsum(Acounts);
Apeaks = findchangepts(Adistribution, 'MaxNumChanges', 10);
A = Avalues(Apeaks(10));
    
[Bcounts, Bvalues] = imhist(b);
Bdistribution = cumsum(Bcounts);
Bpeaks = findchangepts(Bdistribution, 'MaxNumChanges', 10);
B = Bvalues(Bpeaks(10));

dist = abs((a - A)/5).^2 + abs((b - B)/2).^2; % normalize w.r.t. the different ranges in a & b space % this approach is still problematic, can also try: max(abs((a - A)/5).^2, abs((b - B)/2).^2)
dist = mat2gray(dist); % scale to [0,1]


if PLOT
    dist_plot = dist;
end

if BOUNDARY_ENHANCE
    highlight = imbinarize(L, 0.95); % choose the top 5% brightness as highlight
    shadow = ~imbinarize(L, 0.3); % choose the bottom 30% brightness as shadow
    a(highlight) = A; % to avoid introduce artifact, set to the average object value. 
    b(highlight) = B;
    
    % Extract object boundary based on gradient information of a & b channels
    %[LMag, LDir] = imgradient(L); % @note: shouldn't include info from L channel since it will recognize shadow area as boundary
    [aMag, aDir] = imgradient(a);
    [bMag, bDir] = imgradient(b);
    
    % Overlap the boundary information from multiple channels
    % boundary = aMag+bMag; % @note: previous version by adding. Not reasonable because a and b could cancel instead of complementing each other
    rock = max(aMag,bMag);  
    rock = imguidedfilter(rock, 'NeighborhoodSize', 2 * sigma + 1); % this step is important! Guided filter is powerful...it's like double-enhancing the boundary
    boundaryMask = imbinarize(rock);
        
    % Enhance the boundary pixels in the distance map 
    dist(boundaryMask) = 0; 
    
    if PLOT
    figure(fig); fig = fig + 1;
    [ha, pos] = tight_subplot(1,2,[.01 .01],[.01 .01],[.01 .01]);
    axes(ha(1)), imshow(rock), title('Object Boundary');
    axes(ha(2)), imshow(boundaryMask), title('Boundary Mask');
    
    figure(fig); fig = fig + 1;
    [ha, pos] = tight_subplot(1,2,[.01 .01],[.01 .01],[.01 .01]);
    axes(ha(1)), imshow(dist_plot), title('Distance Map, Original');
    axes(ha(2)), imshow(dist), title('Distance Map, Boundary Enhanced');
    end
end

% Multi-level thresholding (using Otsu's method) to segment the rock and calibration ball
% respectively
level = multithresh(dist, 2);
segment_label = imquantize(dist, level);
coloring = label2rgb(segment_label);
figure(fig); fig = fig + 1;
imshow(coloring);

% Binarize the distance map
bw = ~imbinarize(dist); % bw = ~imbinarize(dist, 0.2);
bw(shadow) = 0; % can be omitted, this is used to ensure shadow edge is not recognized as objecct boundary

% Morphological operations on binary image
bw = imdilate(bw, strel('disk', 2 * sigma)); % obtain a closed boundary
bw = imfill(bw, 4, 'holes'); % fill holes inside a connected region, 8-connected is more strict and fill fewer holes
bw = imerode(bw, strel('disk', 2 * sigma)); % imdilate's correspondence
bw = imopen(bw, strel('disk', 2* sigma)); % open: open holes (or remove objects), erode + dilate
bw = bwareaopen(bw, ceil(h/100) * ceil(w/100)); % remove small object
bw = imclearborder(bw, 8); % clear meaningless regions that are connected to image border

if PLOT
figure(fig); fig = fig + 1;
imshow(bw), title('Boundary Mask');
end

[Label,N] = bwlabel(bw, 4); % N is the number of regions
stats = regionprops(Label, 'all'); 
allArea = [stats.Area];
allBoundingBox = [stats.BoundingBox];
allDiameter = [stats.EquivDiameter];

% sort by the region area in descending order to distinguish ball and rock. can also use circular Hough transform to recognize ball 
[data, index] = sort(allArea, 'descend'); 
rockIdx = index(1);
ballIdx = index(2);
rockArea = allArea(rockIdx);
ballArea = allArea(ballIdx);
ballDiameter = allDiameter(ballIdx);

% Get the mask of each object
rockMask = ismember(Label, rockIdx);
ballMask = ismember(Label, ballIdx);

rockCrop = imcrop(rockMask,allBoundingBox(4*(rockIdx-1)+1:4*(rockIdx-1)+4));
ballCrop = imcrop(ballMask,allBoundingBox(4*(ballIdx-1)+1:4*(ballIdx-1)+4));


%% Detect surface holes for volume correction
% https://www.mathworks.com/help/images/examples/marker-controlled-watershed-segmentation.html
[h, w] = size(rockCrop);
radius = min(ceil(0.01 * h), ceil(0.01 * w));
se = strel('disk', radius);

rockMask = imerode(rockMask, strel('disk', 5 * sigma)); % avoid touching the boundary
rock = L;
rock(~rockMask) = 1; % Or: rock = bsxfun(@times, L, cast(rockMask, 'like', L)); % extract the rock object from L channel image
rock = imcomplement(rock); % negative the image to highlight the area of hole shadows
rock = histeq(rock); % contrast enhancement via histogram equalization
% rock(rock >= 0.8) = 1;
% rock(rock <= 0.2) = 0;
% imshowpair(rock, rock_, 'montage');
surface_erode = imerode(rock, se); % erosion is essentially a local-minima convolution kernal, it assign the minimum pixel in the window to the center pixel. Dilation is local-maxima opeartor. This is true for both grayscale and binary image. 
surface_reconstruct = imreconstruct(surface_erode, rock); % image reconstruction is like given a seed location, and dilate many times until it is similar to the target, imreconstruct(seed, target). usually seed is got by erosion (by focusing on the highlight part), and target is usually just the original image. The result is a smoothed/denoised shape-preserving image.
surface_dilate = imdilate(surface_reconstruct, se);
surface_reconstruct = imreconstruct(imcomplement(surface_dilate), imcomplement(surface_reconstruct)); % imreconstruct works on light pixels, so should use complement image
surface_reconstruct = imcomplement(surface_reconstruct);
holes = surface_reconstruct == 1; % Or: holes = imregionalmax(surface_reconstruct);
holes = bwareaopen(holes, ceil(h/100) * ceil(w/100));
imshow(holes);
holeArea = sum(holes(:));
rgb(holes) = 1;
imshowpair(rgb, rock, 'montage');
holeRatio = holeArea / rockArea;

[Label,N] = bwlabel(holes, 4); % N is the number of regions
if N > 0
stats = regionprops(Label, 'Area'); 
partitionArea = [stats.Area];
[data, index] = sort(partitionArea, 'descend'); 
plane = partitionArea(index(1));
weakPlane = min(plane, rockArea - plane);
arrisRatio = weakPlane / rockArea;
else
    arrisRatio = 0;
end
% totalArrisRatio = 1 - (arrisRatio1 * Area1 + arrisRatio2 * Area2 + arrisRatio3 * Area3) / (Area1 + Area2 + Area3);
bd = edge(holes, 'canny');
rockMask = imerode(rockMask, strel('disk', radius));
bd = bsxfun(@times, bd, cast(rockMask, 'like', bd));
figure(1);
imshowpair(rock, bd, 'montage');

[H,T,R] = hough(bd,'RhoResolution',2,'Theta',-90:1:89.5);
% Pick orientations with highest vote.
P  = houghpeaks(H,15,'threshold',ceil(0.5*max(H(:))));
% PLOT Hough transform
figure(2);
imshow(imadjust(mat2gray(H)),'XData',T,'YData',R, 'InitialMagnification','fit');
xlabel('Angle, \theta', 'fontsize',20), ylabel('Distance, \rho', 'fontsize',20);
axis on, axis normal;
colormap(gca, hot);
hold on;
x = T(P(:,2)); y = R(P(:,1));
plot(x,y,'s','color','blue','Markersize', 10, 'linewidth',2);
title('Hough transform statistics (Peaks labelled)','fontsize',20);

lines = houghlines(bd,T,R,P,'FillGap',radius,'MinLength',5*radius);
% PLOT Hough lines
figure(3);
imshow(img), axis on, hold on, title('Line segments detected (Fill gap = 20, Min Length = 100)','fontsize',10);
set(get(gca,'title'),'Position',[640 -20 1])
max_len = 0;
for k = 1:length(lines)
   % Plot line segments 
   xy = [lines(k).point1; lines(k).point2];
   plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

   % Plot beginnings and ends of lines
   plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
   plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');

   % Determine the longest line segment
   len = norm(lines(k).point1 - lines(k).point2);
   lines(k).length = len; % add a new field 'length'
   if ( len > max_len)
      max_len = len;
   end
end

    