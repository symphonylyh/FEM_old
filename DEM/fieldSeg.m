% File Dependencies: rgb2lab.m, tight_subplot.m, getAllFilesInFolder.m

%% Control panel
SEGMENT = false;
RESIZE = true;

%% Read image folder
% Specify the working folder and get all image files in it
inFolderName = './samples/Apr_25_2018/'; % user-define
fnames = getAllFilesInFolder(inFolderName); % this function is tuned to ignore any additional folder such as "Segmentation" and "Resizing"
if (mod(length(fnames),3) ~= 0)
    error("Images are not paired in triplet...Abort!");
end

% Format arbitrary image file names to img000N_X 
% where N = image No. and X = 0(top)/1(front)/2(side) 
% Note: the sequence of raw images should be taken in front-->side-->top!
% The filename formatting should only be done once for a folder
if fnames{1}(1:3) ~= 'img' % if already formatted, skip
    % Get the file extension
    [path, name, extension] = fileparts(fnames{1}); 
    for i = 1 : length(fnames)
        % Rename file
        movefile(fullfile(inFolderName, fnames{i}), fullfile(inFolderName, strcat('img', sprintf('%04d', ceil(i / 3)), '_', num2str(mod(i, 3)), extension)));
    end
    % Update the recorded file name if they are renamed
    fnames = getAllFilesInFolder(inFolderName);
end

% Create output folder
outFolderName = strcat(inFolderName, 'Segmentation/');
if ~exist(outFolderName, 'dir')
	mkdir(outFolderName);
end


%% Segmentatioin
if SEGMENT
    %img = illiSeg(fullfile(inFolderName,fnames{21})); % single image
    summary = [];
    for object = 1 : length(fnames) / 3
        results = [];
        for i = 1 : 3 % image triplet of top-front-side views of an object
            views{i} = fullfile(inFolderName, fnames{(object - 1) * 3 + i});
            results = cat(2, results, illiSeg(views{i}));
        end
        summary(2*(object-1)+1:2*(object-1)+2, :) = results;
        % summary compiles particle information of all the segmented images
        % Columns are the bounding box dimensions of the rock particle
        % expressed in x-y/width-height, three views stacked following the
        % top-front-side sequence:
        % width0(x) height0(y) width1(x) height1(y) width2(x) height2(y) 
        % Rows are interleaved as rock-ball pair for one set of photos:
        % set1_rock
        % set1_ball
        % set2_rock
        % set2_ball
        % ...
    end
    % Save the summary info to disk
    save(fullfile(outFolderName, 'summary.mat'), 'summary');
end

%% 3D volume estimation
close all;
if RESIZE
    S = load(fullfile(outFolderName, 'summary.mat'), '-mat');
    info = S.summary;
    nums = size(info,1) / 2; % number of particle sets
    
    % Calculate the benchmarked dimensions (x,y,z) from the least squares 
    % solution of the linear system
    for i = 1:6
        for j = 1 : 3
            rocks{j} = imread(fullfile(inFolderName, 'Segmentation/', strcat('timg', sprintf('%04d', i), '_', num2str(j - 1), '_rock.png')));
            balls{j} = imread(fullfile(inFolderName, 'Segmentation/', strcat('timg', sprintf('%04d', i), '_', num2str(j - 1), '_ball.png')));
            D(j) = info(2 * i, 2 * j - 1);
        end
        rockVoxel = reconstruct3D(rocks, D);
        ballVoxel = reconstruct3D(balls, D);
        rockVolume = rockVoxel / ballVoxel * 0.523599 * 16.3871; % calibration ball is V = 4/3 * PI * R3 = 0.523599 in3; 1 in3 = 16.3871 cm3
        % rockVolume = rockVoxel / ballVoxel * 1^3 * 16.3871 / 2; % calibration ball is V = 1 in3; 1 in3 = 16.3871 cm3
        rockWeight = rockVolume * 2.65; % typically rock density 2.65g/cm3
        volumes(i, 1) = rockVolume;
        weights(i, 1) = rockWeight;
        % Save the 3D voxel array to disk
        % save(fullfile(outFolderName, 'volume.mat'), 'volume');
        
    end
    
    weights(:, 2) = [3175.15; 2487.7; 2463.9; 2955.1; 2235.8; 1712.5]; % old measure
    weights(:, 2) = [3214.9; 2487.7; 2463.9; 2955.1; 2235.8; 1712.5]; % new measure
    volumes(:, 2) = [1254.8; 916.4; 947.8; 1149.6; 871.7; 636.3]; % submerge measure
    error = (volumes(:,1) - volumes(:,2)) ./ volumes(:,2) * 100; % percentage
    figure;
    plot(weights(:,2), weights(:,1), '*r'), xlim([1000 4000]), ylim([1000 4000]), refline(1, 0);
    % plot(volumes(:,2), volumes(:,1), '*r'), xlim([500 1500]), ylim([500 1500]), refline(1, 0);
    xlabel('Actual Volume (in cm3)'), ylabel('Reconstructed Volume (in cm3)');
    % Create output folder for the resized particle image
    outFolderName = strcat(inFolderName, 'Resizing/');
    if ~exist(outFolderName, 'dir')
        mkdir(outFolderName);
    end
    
    
end

%% Notes
% Volume visualization tutorial:
% https://blogs.mathworks.com/videos/2009/10/23/basics-volume-visualization-19-defining-scalar-and-vector-fields/
% https://stackoverflow.com/questions/2942251/matlab-3d-volume-visualization-and-3d-overlay
% https://stackoverflow.com/questions/13553108/how-i-can-display-3d-logical-volume-data-matlab
% https://stackoverflow.com/questions/6891154/creating-3d-volume-from-2d-slice-set-of-grayscale-images
% 
% Isosurface: isosurface(V, value)
% https://www.mathworks.com/help/matlab/ref/isosurface.html
% 
% 3D binary boundary of mask:
% https://www.mathworks.com/matlabcentral/answers/85180-multi-dimensional-version-of-bwboundaries
%
% 3D boundary of a set of points: boundary(x,y,z)
% https://www.mathworks.com/help/matlab/ref/boundary.html
%
% 3D Minkowski geometric measures: imMinkowski package
% https://www.mathworks.com/matlabcentral/fileexchange/33690-geometric-measures-in-2d-3d-images
%
% 3D voxel rendering: vol3d package
% https://www.mathworks.com/matlabcentral/fileexchange/22940-vol3d-v2
% https://blogs.mathworks.com/pick/2013/10/04/easy-visualization-of-volumetric-data/
% 
% 3D reconstruct:
% https://www.mathworks.com/matlabcentral/fileexchange/3280-voxel
% https://www.mathworks.com/matlabcentral/fileexchange/42876-surf2solid-make-a-solid-volume-from-a-surface-for-3d-printing?focused=3810976&tab=function
% https://www.mathworks.com/matlabcentral/fileexchange/24484-geom3d?focused=8549167&tab=function
% https://www.mathworks.com/matlabcentral/fileexchange/37268-3d-volume-visualization
% https://www.mathworks.com/matlabcentral/fileexchange/59161-volumetric-3?s_tid=srchtitle
% https://www.mathworks.com/help/images/explore-3-d-volumetric-data-with-volume-viewer-app.html