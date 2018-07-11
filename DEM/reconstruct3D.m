function [voxel, sphericity] = reconstruct3D(views, D, debug_mode)
% 3D reconstruction based on three near-orthogonal views.
%
% Input:
% views: 1x3 cell array containing three views (binary images, in sequence top-front-side)
% D: 1x3 vector containing three equivalent diameters of calibration ball
% in each view
%
% Output:
% voxel: voxel counter of the reconstructed object

close all;

PLOT = debug_mode;
PLOT = false;

% Normalize/Scale with respect to the *top* view based on the diameter ratio of calibration ball
views{2} = imresize(views{2}, D(1) / D(2));
views{3} = imresize(views{3}, D(1) / D(3));

% Update the new image dimension in info table
% info(2 * i - 1, 2 * 2 -1 : 2 * 2) = [size(views{2}, 2) size(views{2}, 1)];
% info(2 * i - 1, 2 * 3 -1 : 2 * 3) = [size(views{3}, 2) size(views{3}, 1)];

% Following the sequence of photos top-front-side
% a right-hand coordinates system is used:
% -------- +x
% |
% |
% |
% |
% +y
% and positive z is pointing into the screen
% in the image sets taken April 25th, each row in summary matrix
% is [z x x y z y]

% The target is to find a benchmark dimension [x0 y0 z0] for three views to 
% be scaled with, thereby voxels can be calculated. The method is to solve
% a least squares problem of the linear system.
% Ax ~= b
% A       *     x      =  b    
% | 0  0  1 |  | x0 |     | z |  top_width
% | 1  0  0 |  | y0 |     | x |  top_height
% | 1  0  0 |  | z0 |     | x |  front_width
% | 0  1  0 |             | y |  front_height
% | 0  0  1 |             | z |  side_width
% | 0  1  0 |             | y |  side_height
%    +z________________
%     /               /|
%    /      p        / |
%   /      o        /  |
%  /      T        /   |
% /_______________/+x  /
% |               |   /  e
% |               |  /  d
% |    Front      | /  i
% |_______________|/  S
% +y
% The text direction shows the way the photo is took
% Restrict x = y = z for calibration ball
% | 1  -1  0 |             | 0 |  x ~= y
% | 0  1  -1 |             | 0 |  y ~= z
% | 1  0  -1 |             | 0 |  x ~= z

% Solve linear system
for i = 1 : 3
    b(2 * i - 1, 1) = size(views{i}, 2); % width
    b(2 * i, 1) = size(views{i}, 1); % height
end
% b = info(2 * i - 1, :)';
A = [0 0 1; 1 0 0; 1 0 0; 0 1 0; 0 0 1; 0 1 0];

if (size(views{1}, 1) < 100) % ball
    b(7:9, 1) = [0 0 0]';
    A(7:9, :) = [1 -1 0; 0 1 -1; 1 0 -1];
end
scale = ceil(A \ b); % [x y z]
top = imresize(views{1}, [scale(1) scale(3)]);
front = imresize(views{2}, [scale(2) scale(1)]);
side = imresize(views{3}, [scale(2) scale(3)]);

distortion = @(xyz) ( abs(xyz(1) - xyz(2)) + abs(xyz(2) - xyz(3)) + abs(xyz(1) - xyz(3)) ) / mean(xyz); 
sphericity = 1 - distortion(scale); % calculate the sphericity of the captured calibration ball, 1 for perfect sphere

% Extrude and rearrange into [x y z] dimension
top_extrude = repmat(top, [1 1 scale(2)]); % [x z y]
top_extrude = permute(top_extrude, [1 3 2]);
front_extrude = repmat(front, [1 1 scale(3)]); % [y x z]
front_extrude = permute(front_extrude, [2 1 3]);
side_extrude = repmat(side, [1 1 scale(1)]); % [y z x]
side_extrude = permute(side_extrude, [3 1 2]);

% Intersect the three extruded body
volume = top_extrude & front_extrude & side_extrude;

voxel = sum(volume(:));
% rockVolume = voxel / D(1)^3 * 1^3; % in in^3
% rockWeight = rockVolume * 16.3871 * 2.65; % 1 in3 = 16.3871 cm3; typically rock density 2.65g/cm3
% volumes(i, 1) = rockVolume * 16.3871;
% weights(i, 1) = rockWeight;
% Save the 3D voxel array to disk
% save(fullfile(outFolderName, 'volume.mat'), 'volume');

if PLOT
% figure(1), imshow(top);
% figure(2), imshow(front);
% figure(3), imshow(side);
% 
% % Show images on a 3D cube: https://www.mathworks.com/matlabcentral/answers/32070-rgb-images-on-a-3d-cube
figure(4);
% top
surface([0 scale(3); 0 scale(3)], [0 0; scale(1) scale(1)], [scale(2) scale(2); scale(2) scale(2)], 'FaceColor', 'texturemap', 'CData', flip(double(top)));
% front
surface([0 scale(3); 0 scale(3)], [0 0; 0 0], [0 0; scale(2) scale(2)], 'FaceColor', 'texturemap', 'CData', flip(double(side)));
% left
flip_front = imrotate(double(front), 90);
flip_front = flip(flip_front, 1);
flip_front = flip(flip_front, 2);
surface([0 0; 0 0], [scale(1) scale(1); 0 0], [0 scale(2); 0 scale(2)], 'FaceColor', 'texturemap', 'CData', flip_front);
axis equal
view(3);

%volume = permute(volume, [1 2 3]);
%         [Rx Ry Rz] = size(volume); % the reconstruct coordinates system used above
%         [Vx Vy Vz] = meshgrid(1:Rz, 1:Rx, 1:Ry); % rearrange the axis to Matlab plot's right-handed system
        
        % Plot 3d view   
        figure(5);
        % adjust the coordinates
        volume = flip(volume, 1); % flip x and y
        volume = flip(volume, 2);
        volume = permute(volume, [1 3 2]); % exchange y and z
        v = double(volume);
        p = patch( isosurface(v,0) );                 %# create isosurface patch
        isonormals(v, p)                              %# compute and set normals
        set(p, 'FaceColor','r', 'EdgeColor','none')   %# set surface props
        daspect([1 1 1])                              %# axes aspect ratio
        view(0, 90), axis vis3d tight, box on, grid on    %# set axes props
        set(gca, 'CameraUpVector', [0 1 0])
        set(gca, 'CameraUpVectorMode', 'manual')
        rotate3d on
        camproj perspective                           %# use perspective projection
        camlight('left'), lighting phong, alpha(1)
        % end plot 3d view

%         
%         surfaceVoxels = volume - imerode(volume, true(3));
%         
%         vol3d('cdata',volume,'texture','3D');
%         view(45,15);  axis tight;axis off;
%         camlight; camlight(-90,-10); camlight(180,-10);lighting phong;
%         alphamap('rampup');
%         alphamap(0.05 .* alphamap);
%         figure;
%         cmap = [0 0 1];
%         hpat = PATCH_3Darray(volume, cmap);

end

end % end of function