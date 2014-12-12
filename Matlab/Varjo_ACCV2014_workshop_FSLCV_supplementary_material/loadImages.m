function I = loadImages(folder, namemask, filetype, verbose)

%function I = loadImages(folder, namemask, filetype, verbose)
%  if ~exist('folder','var'), folder = '.'; end
%  if ~exist('namemask','var'), namemask= '*'; end
%  if ~exist('filetype','var'), filetype= 'tif'; end
%  if ~exist('verbose','var'), verbose = 0; end

if ~exist('folder','var'), folder = '.'; end
if ~exist('namemask','var'), namemask= '*'; end
if ~exist('filetype','var'), filetype= 'tif'; end
if ~exist('verbose','var'), verbose = 0; end

searchPattern = sprintf('%s/%s.%s',folder,namemask,filetype);
files = dir( searchPattern );

nFiles = length(files);

if nFiles == 0
    warning('No files %s was found', searchPattern');
    return
end

if verbose
    fprintf('#images %d : ',nFiles);
end

I = cell(nFiles,1);
for fId = 1:nFiles 
    fullName = sprintf('%s/%s', folder, files(fId).name);
    I{fId} = imread( fullName  );
end

end