fprintf('Supplementary material for Image Based Visibility During Day and Night\n\n')
%% 
%Generate the HDR images
HDRpath = './HDR_images/';

createHDRimages=0;

if createHDRimages
    processFolders('20*','*','autoSum16bit',0,{ HDRpath , 1, 14  },6);
else
    fprintf('The original images are losslessly compressed 16bit images and \n');
    fprintf('the four exposure stacks demonstrated here would had required 81MB\n'); 
    fprintf('of strorage space, which is limited to 30MB for supplementary material.\n');
    fprintf('Therefore, ready HDR images are provieded.\n');
end
%%

[imFile, imTime] = listImageFiles( HDRpath ,'tif');

N=length(imFile);
I = zeros(960,1280,N);
Ir = zeros(952,1272,N);
v  = zeros(N,951);
for id = 1:length(imFile)    
    I(:,:,id) = normaliseToDouble(loadVisImage( id, imFile, HDRpath));
    Ir(:,:,id) = multiScaleRetinex( I(:,:,id) );
    v(id,:) = makeFeatureVector( I(:,:,id) );
end

%%
figure, 
subplot(431), imshow( adapthisteq(I(:,:,1)));  title( datestr(imTime(1)) );
subplot(432), imshow( normaliseToDouble(Ir(:,:,1)) ); title('Retinex filtered');
subplot(433), plot( v(1,:) ); view([90 90]); title('Y-projection');

subplot(434), imshow( adapthisteq(I(:,:,2)));  title( datestr(imTime(2)) );
subplot(435), imshow( normaliseToDouble(Ir(:,:,2)) ); 
subplot(436), plot( v(2,:) ); view([90 90])

subplot(437), imshow( adapthisteq(I(:,:,3))); title( datestr(imTime(3)) );
subplot(438), imshow( normaliseToDouble(Ir(:,:,3)) );  
subplot(439), plot( v(3,:) ); view([90 90])

subplot(4,3,10), imshow( adapthisteq(I(:,:,4)));  title( datestr(imTime(4)) );
subplot(4,3,11), imshow( normaliseToDouble(Ir(:,:,4)) ); 
subplot(4,3,12), plot( v(4,:) ); view([90 90])

drawnow
pause(0.1);
%%
fprintf('\nNext the pre-extracted features are used for training SVMs.\n');
fprintf('Please wait...\n\n');

run trainSVMs_HDRvisMeData.m
