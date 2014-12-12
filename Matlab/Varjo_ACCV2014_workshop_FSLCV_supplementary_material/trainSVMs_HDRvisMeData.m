if ~exist('svmtrain')
    error('svmtrain missing from Matlab - Statistics toolbox is required!');
end

%%
crossValN = 5;  %N-fold hold out for validation (ie 1/N used for testing rest training)

%for expId = 1:10 %
for expId = 1
    
    load labelData %a partition to test and train sets
    
    dataSetList{13} = 'Matilda-retinex-y-proj-grad';
    dataSetList{14} = 'Matilda-retinex-y-proj-grad-day_2';
    dataSetList{15} = 'Matilda-retinex-y-proj-grad-night_2';
        
    dataSetList{17} = 'HDRvisMe-retinex-y-proj-grad';
    dataSetList{18} = 'HDRVisMe-retinex-y-proj-grad-5h';
    dataSetList{19} = 'HDRVisMe-retinex-y-proj-grad-5h-day';
    dataSetList{20} = 'HDRVisMe-retinex-y-proj-grad-5h-night';
    dataSetList{21} = 'HDRVisMe-retinex-y-proj-grad-5h-trainDay_testNight';
    
    runSet = [18];
    
    %%
    %Params for grid search
    %
    %  rbf_sigma_set = 1.25.^[-3:8];
    %  Cset = 1.75.^[-3:8];
    
    rbf_sigma_set = 1.25.^2;
    Cset = 1.75.^3;
    
    autoscale = [false];  %AUTO SCALE MUST BE FALSE!! with true - nothing works!!
    
    %Which kernel to use:
    svmkernelList = {'rbf';};
    %svmkernelList = {'linear';'quadratic';'polynomial';'rbf';'mlp'};

    useHellinger = 0; %not a good idea here perf drops ~15%
        
    for dSetId = runSet;
        
        dataSet = dataSetList{dSetId};
        
        if dSetId == 14 || dSetId == 15
            load(dataSetList{13});
        elseif dSetId == 17
            load(dataSetList{11});
            im_VisGt = im_VisGT;
        elseif dSetId ==18 || dSetId == 19 || dSetId == 20 || dSetId == 21
            load(dataSetList{17})
            im_VisGt = im_VisGT;
        else
            load(dataSet);
        end
        
        
        if dSetId == 14            
            dayId = im_lum(:,2) > 10;
            featureVector = featureVector(dayId,:);
            label = label(dayId);            
        end
        
        if dSetId == 15            
            nightId = im_lum(:,2) < 10;
            featureVector = featureVector(nightId ,:);
            label = label(nightId );            
        end
        
        
        if dSetId == 16
            load f:/Data_Kumpula/vaisala_vd22_vis/okDat.mat;
            im_VisGt = im_VisGT;
            clear im_VisGT;
        end
        
        if dSetId == 17
            im_VisGt = im_VisGT;
        end
                
        if dSetId == 19
            featureVector = featureVector(dayId,:);
            label = label(dayId);
        end
        
        if dSetId == 20
            featureVector = featureVector(nightId ,:);
            label = label(nightId );
        end
        
        
        %%
        N = length(label);
        for id = 1:Nclasses %number of samples in each class
            sumCtot(id) = sum( label == classLabel(id) );
        end
                
        holdFact = 1/crossValN;
        holdSumC = floor(sumCtot.*holdFact);
        
        %Must be recalculated if separated to day/night (not whole)
        idC1 = find(label==classLabel(1));
        idC2 = find(label==classLabel(2));
        idC3 = find(label==classLabel(3));
        idC4 = find(label==classLabel(4));
        idC5 = find(label==classLabel(5));
        
        %Dummy for try catch if there is training convergence error
        testId = [ idC1(randi([1 sumCtot(1)],[holdSumC(1) 1] ))...
            idC2(randi([1 sumCtot(2)],[holdSumC(2) 1] ))...
            idC3(randi([1 sumCtot(3)],[holdSumC(3) 1] ))...
            idC4(randi([1 sumCtot(4)],[holdSumC(4) 1] ))...
            idC5(randi([1 sumCtot(5)],[holdSumC(5) 1] ))];
        
        trainId = setdiff(1:N,testId);

        trainData = featureVector(trainId,:);
        labelTrain = label(trainId);
        labelTest = label(testId);
        testData = featureVector(testId,:);
        
        NtotTrain = length(labelTrain);
        NtotTest = length(labelTest);
        
        clear F1_C_sigma; %Ensure right size (if params changed)
        
        runId = 1;
        
        if dSetId == 5 %Contains combined features (single value for Peli and Gradient with different normalization overlaps)
            %do gradient based
            featureVector=featureVector(:,floor(size(featureVector,2)/2)+1:end);
            %do PeliContrast
            %featureVector=(:,1:floor(size(featureVector,2)/2));
        end
        
        if useHellinger
            featureVector = sqrt(featureVector);
        end
                
        featureVectorOrig = featureVector; %Working backup        
        %%
        
        [pca_coeff,score,latent,tsquared,explained,mu] = pca(featureVectorOrig);
        
        
        %for explainedLimit = 0.99
        for explainedLimit = 1.000; %
        %for explainedLimit = [0.9:0.01:1.0]
            
            pcaIdx = find( (cumsum(latent)./sum(latent)) >= explainedLimit , 1 ,'first'); %Latent contains variance of each pca (why not to use explained...)
            loopIdx = 0;
            clear TPper FNper FPper
            
            for selectN_best =  pcaIdx;
                
                featureVector = featureVectorOrig*pca_coeff(:,1:pcaIdx); 
                
                clear SVMStruct;
                
                for svmkernelId = 1:length(svmkernelList) ;
                    
                    svmkernel = svmkernelList{svmkernelId};
                    
                    for Cid =1:length(Cset);
                        C = Cset(Cid);
                        rbf_sigma_id = 0;
                        
                        
                        for rbf_sigma_id = 1:length(rbf_sigma_set); %Loop for ROC curve
                            rbf_sigma = rbf_sigma_set(rbf_sigma_id);
                            
                            loopIdx = loopIdx + 1; %obsolete
                           
                            for polyOrder = [3] %unused
                                
                                for tolkkt = [1e-6]                     %may affect convergence 
                                    for kktviloationlevel = [ 1e-6 ]    %(machine prec ~1e-14)
                                        
                                        % % % % %
                                         try
                                            
                                            fprintf('training with %s kernel kernSigma:%d C:%u\n',svmkernel,rbf_sigma,C)
                                            %%
                                            tTime = 0;
                                            for setId = 1:Nclasses
                                                                                                                                               
                                                
                                                fprintf('Class %u of %u\n', setId,Nclasses );
                                                
                                                
                                                group = labelTrain;
                                                group(group ~= classLabel(setId) ) = 0;

                                                opt_smo = statset('Display','off','Maxiter',30000); %for SMO
                                                
                                                SVMStruct(setId) = svmtrain(trainData,group,'autoscale',autoscale,... %keep false
                                                    'kernel_function',svmkernel,...               %
                                                    'kktviolationlevel',kktviloationlevel,...
                                                    'method','SMO',...
                                                    'options',opt_smo,...
                                                    'tolkkt', tolkkt,...
                                                    'rbf_sigma',rbf_sigma,...
                                                    'boxconstraint',C );%,...
                                                
                                                %'polyorder',polyOrder );
                                                
                                                % %     opt_qp = optimset('Algorithm','interior-point-convex') %for QP
                                                % %     SVMStruct(setId) = svmtrain(trainData,group,'autoscale',autoscale,...
                                                % %                                 'kernel_function','rbf',...
                                                % %                                 'kktviolationlevel',kktviloationlevel,...
                                                % %                                 'method','QP',...
                                                % %                                 'options',opt_smo,...
                                                % %                                 'rbf_sigma',rbf_sigma,...
                                                % %                                 'tolkkt', tolkkt );
                                                

                                            end
                                            
                                            
                                            %%
                                            %test
                                                                  
                                            res = zeros(size(testData,1),length(Nclasses) );
                                            conf = zeros(size(testData,1),length(Nclasses) );
                                            
                                            for cId = 1:Nclasses
                                                [class, cert] = svmClassifyWithCertainty( SVMStruct(cId), testData, [0 classLabel(cId)] ); %One against all
                                                conf(:,cId) = cert;
                                                res(:,cId) = class';
                                            end
                                            
                                            
                                            [v, id] = max(conf, [],2);     %Winner takes all
                                            for row = 1:size(res,1)
                                                singleRes(row)=res(row,id(row));
                                            end
                                            
                                            [ConfMat, confOrder] = confusionmat(labelTest,singleRes);
                                            if confOrder(1)==0
                                                ConfMat = ConfMat(2:end,2:end);
                                            end
                                            
                                            for id = 1:Nclasses
                                                sumC(id) = sum( labelTest == classLabel(id) );
                                            end
                                            
                                            %plotroc(labelTest,singleRes)
                                            TP = trace(ConfMat);
                                            FN = sum(sumC - diag(ConfMat)');
                                            FP = sum( sum( ConfMat, 2) - diag(ConfMat) );
                                            
                                            F1_Measure = 2*TP / ( 2*TP + FP + FN  );
                                            F1(runId) = F1_Measure;
                                            
                                            F1_C_sigma(Cid,rbf_sigma_id) = F1_Measure;
                                            
                                            
                                            clear TP FN FP RECALL ACCURACY PRECICION F1_Measure;
                                            for setId = 1:Nclasses
                                                
                                                posIdx = (labelTest==classLabel(setId));
                                                negIdx = (labelTest~=classLabel(setId));
                                                
                                                TP(setId) = sum( res(posIdx,setId) == classLabel(setId)  );
                                                FN(setId) = sum( res(posIdx,setId) ~= classLabel(setId)  );
                                                FP(setId) = sum( res(negIdx,setId) == classLabel(setId) );
                                                
                                            end
                                            
                                            %%%%%%
                                         catch %training failed most probably (no convergence)
                                             warning('Training Failed')
                                             TP = 0;
                                             FN = sum(NtotTrain);
                                             FP = 0;
                                             ConfMat = zeros(Nclasses);
                                             F1(runId) = 0;
                                             F1_C_sigma(Cid,rbf_sigma_id) = 0;
                                         end
                                        
                                        TPper(loopIdx,:) = squeeze( round( (TP./sumC) * 1000 )/10 );
                                        FNper(loopIdx,:) = squeeze( round( (FN./sumC) * 1000 )/10 );
                                        FPper(loopIdx,:) = squeeze( round( (FP./sum(sumC)) * 1000 )/10 );
                                        %%
                                        
                                        if ~exist('confFig')
                                            confFig = figure;
                                        end
                                        
                                        figure(confFig);
                                        imagesc(ConfMat./repmat( sum(ConfMat,2), 1,length(classLabel) ),[0 1]);
                                        axis image
                                        colorbar
                                        set(gca,'XTick',1:length(classLabel))
                                        set(gca,'YTick',1:length(classLabel))
                                        set(gca,'XTickLabel',classLabel);
                                        set(gca,'YTickLabel',classLabel);
                                        xlabel('Assigned Class');
                                        ylabel('True Class');
                                        
                                        switch svmkernel %'linear';'quadratic';'polynomial';'rbf';'mlp'
                                            case 'rbf'
                                                suptitle(sprintf('%s kernel | kernSigma:%d | C:%d | Nfeat:%d | F1:%.3f',svmkernel, rbf_sigma, C, selectN_best,F1(runId) )) ;
                                            case 'polynomial'
                                                suptitle(sprintf('%s kernel | polyorder:%d | Nfeat:%d | F1:%.3f',svmkernel, polyOrder,selectN_best,F1(runId) )) ;
                                            otherwise
                                                suptitle(sprintf('%s kernel | Nfeat:%d | F1:%.3f',svmkernel,selectN_best,F1(runId)  )) ;
                                        end

                                            
                                        %%
                                        folderBase = sprintf('./%s_%s_pca%u_exp%u/',dataSet,'confClass',uint8(explainedLimit*100),expId);
                                        folder = sprintf(folderBase);
                                        
                                        if ~exist(folder,'dir')
                                            mkdir(folder)
                                        end
                                        
                                        filename = sprintf('%s%uF1_run_%u_%s_sN%u_H%u.jpg',folder,uint8(F1(runId)*100),runId,svmkernel,selectN_best,useHellinger);
                                        saveas(confFig,filename,'jpg');
                                        
                                        runId= runId + 1;
                                                                                
                                    end
                                end
                            end
                        end
                    end
                end
            end
                  
            %%
            diaryName = sprintf('./%s/diary_%s.txt',folderBase,dataSet);
            diary(diaryName);
            diary on;
            
            fprintf('dataSet: %s\n',dataSet);
            fprintf('pca explained limit: %f\n',explainedLimit);
            fprintf('N features after pca: %u\n',pcaIdx);
            fprintf('Use Hellinger-kernel on data: %u\n\n',useHellinger);
            
            fprintf('SVM kernel: %s\n',svmkernel)
            fprintf('tolkkt %f  kktviolationlevel %f autoscale:%u\n',tolkkt,kktviloationlevel,autoscale);
            fprintf('N trainset tot: %u\n',NtotTrain);
            fprintf('N testset  tot: %u\n',NtotTest);
            fprintf('N classes: %u\n', Nclasses);
            fprintf('class labels: ');
            
            for id = 1:length(classLabel), fprintf('%u ',classLabel(id)); end
            
            fprintf('\n\ngridSearch C(row) vs rbf_sigma(col) F1-measure');
            
            [rId,cId]=ind2sub(size(F1_C_sigma), find(F1_C_sigma==max(F1_C_sigma(:))));
            fprintf('max F1-measure: %f @(%u,%u) == (%f,%f)\n', F1_C_sigma(rId,cId),rId,cId,Cset(rId),rbf_sigma_set(cId));
            
            fprintf('C:\n')
            for id = 1:length(Cset), fprintf('%f\t',Cset(id)); end
            fprintf('\nrbf_sigma:\n')
            for id = 1:length(rbf_sigma_set), fprintf('%f\t',rbf_sigma_set(id)); end
            fprintf('\n\nC vs rbf_sigma data (F1_C_sigma):');
            
            [rc, cc, dc] = size(F1_C_sigma);
            for r = 1:rc
                fprintf('\n');
                for c = 1:cc
                    fprintf( '%f\t', F1_C_sigma(r,c));
                end
            end
            fprintf('\n');
            
            fprintf('\nLast confusion matrix:\n');
            [rc, cc, dc] = size(ConfMat);
            for r = 1:rc
                fprintf('\n');
                for c = 1:cc
                    fprintf( '%f\t', ConfMat(r,c));
                end
            end
            
            fprintf('\n');
            
            diary off;
            
            
            %%
%             if ~exist('figGridSearch','var')
%                 figGridSearch = figure;
%             end
%             figure(figGridSearch);
%             F1plot = (F1_C_sigma).^1;
%             %F1plot = F1plot ./ max(F1plot(:));
%             
%             imagesc( F1plot ),colorbar, hold on; plot(cId,rId,'go');
%             
%             set(gca,'XTick',1:length(rbf_sigma_set))
%             set(gca,'YTick',1:length(Cset))
%             
%             set(gca,'XTickLabel',rbf_sigma_set(1:1:end));
%             xlabel('\sigma_{RBF}');
%             set(gca,'YTickLabel',Cset);
%             ylabel('soft margin - C');
%             
%             title('F1 grid search');
%             
%             fname = sprintf('./%s/GridSearch_%s.png',folderBase,dataSet);
%             saveas(figGridSearch,fname,'png');
            
            
        end 
    end     
end
%%