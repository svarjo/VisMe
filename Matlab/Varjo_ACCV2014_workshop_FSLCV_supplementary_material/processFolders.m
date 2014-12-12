function processFolders( folderPrefix, fileMask, op, useDiary, varargs, expTime )
%processFolders( folderPrefix, fileMask, op, useDiary, varargs )
%   folderPrefix = 'ImgSet'; end
%   fileMask = '*'; end
%   useDiary=0; end
%
% processFolders('20*','*','autoSum16bit',0,{ './HDR_images', 1, 14  },1);

if ~exist('folderPrefix','var'), folderPrefix = 'ImgSet'; end
if ~exist('fileMask','var'), fileMask = '*'; end
if ~exist('useDiary','var'), useDiary=0; end

if ~exist('expTime','var'), expTime = [25 50 100 300 900 2700 8100 24300 72900 218700 656100 1968300 5904900 11809800 23619600 47239200 47239200 70858800 70858800]; end

%Sum image range
if length(varargs)>=2
    startId = varargs{2};
else 
    startId = 1;
end

if length(varargs)>=3
    endId = varargs{3};
end

searchPattern = sprintf( '%s*', folderPrefix );
folders = dir( searchPattern )

switch op
    
    case {'sum16bit','sum32bit','sumExpTimeWeighted','autoSum16bit'}

        gamma = 1;
        
        if isempty(varargs),             
            warning('Output folder not given, using current - hit enter to continue')
            outfolder = '.';
            pause();
        else
            outfolder = varargs{1}
        end
        
        if ~exist(outfolder,'dir')
            mkdir(outfolder)
        end
        
        if useDiary
            diaryName = sprintf('%s/log.txt',outfolder);
            diary(diaryName);
            diary on;
        end
        
        for fId = 1:length(folders)
            
            if useDiary
                fprintf('%s : ', folders(fId).name);
            end
            
            Iset = loadImages(folders(fId).name,fileMask,'tif',useDiary);
            
            if ~exist('endId','var')
                endId = length(Iset);
            end

            outnameSuffix = '.tif';
            outName = sprintf('%s%s%s', outfolder, folders(fId).name, outnameSuffix)
                        
            switch op
                case 'sum16bit'   
                    sumI = sumCellImage(Iset,startId,endId);
                    outI = normaliseTo16bit(sumI,gamma, useDiary);
                    imwrite(outI, outName, 'Compression', 'deflate');
            
                case 'sum32bit'
                    sumI = sumCellImage(Iset,startId,endId);
                    outI = normaliseTo32bit(sumI,gamma, useDiary);
                    save32bitTiff(outI,outName);
                    
                case 'sumExpTimeWeighted'                     
                    
                    sumI = zeros(size(Iset{1}));
                    for id = 1:length(Iset)                        
                        %Detect bad over exposure and stop when detected (no more info)
                        h = imhist(double(Iset{id})./max(max(double(Iset{id}))));
                        cdf = cumsum(h);
                        cdf = cdf./max(cdf);
                        if cdf(end)-cdf(end-1) >= .50;                          
                            break;
                        end
                        sumI = sumI + expTime(id).*double(Iset{id});
                        
                    end
                    sumI = sumI./max(sumI(:));
                    minV = min(sumI(:));
                    maxV = max(sumI(:));
                    range = maxV-minV;
                    sumI = (sumI - minV)./range;
                    max16b =  65535;
                    outI = uint16( max16b .* sumI );
                    imwrite( outI, outName, 'Compression', 'deflate');                      
       
                    
                case 'autoSum16bit'                                  
                    [sumI,nImages] = autoSumCellImage(Iset(1:14));
                    nImagesUsedAutoSum(fId) = nImages;                    
                    minV = min(sumI(:));
                    maxV = max(sumI(:));
                    range = maxV-minV;
                    sumI = (sumI - minV)./range;
                    max16b =  65535;
                    outI = uint16( max16b .* sumI );
                    
                    if useDiary
                        fprintf(' use %d images | min=%d max=%d range=%d : ', nImages, minV, maxV, range);
                    end                    
        
                    imwrite( outI, outName, 'Compression', 'deflate');
                    
                    
                otherwise
                    error('processFolders: Unsupported operation: %s\n', op);
            end
            
            if useDiary
                fprintf('\n');
            end
           
            if exist('nImagesUsedAutoSum','var');                
                save nImagesUsedAutoSum nImagesUsedAutoSum
            end
            
        end
   

end

diary off;
end