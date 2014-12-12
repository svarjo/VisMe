function I = loadVisImage( id, imFile, imfolder )
%I = loadVisImage( id, imFile, imfolder )
%
%   Wrapper to read image with given id 
%   see also (getImageVisibility.m, listImageFiles.m)
%

if ~exist('imfolder','var')    
    imfolder = './HDR_images/';
end

fname = [imfolder imFile(id).name];
I = imread(fname);


end