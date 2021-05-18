function Data = MBMS_fun_r(Data,r,sigma,N_iter)
close all
%     if nargin==0
%         load('cube.mat')
load('data.mat')
Data = data ;


k = 10 ;
% n = 2 ; %number of radius
Thr = 0.01 ; % threshold for denoising
% N_iter = 10;
% sigma = 1 ;
twosigmatwo = 2*sigma^2;%abst
% r = 1;%;0.4 + 1 * rand(n,1) ;

%     end
MaxMin = [min(Data(:,1)),max(Data(:,1)) ; min(Data(:,2)), max(Data(:,2)) ; min(Data(:,3)), max(Data(:,3))] ;
for iter=1:N_iter

    Data_New = zeros(size(Data,1), size(Data,2)) ;%abst
%     for j=1:n
        % finding neighbors
        [Index,~] = rangesearch(Data,Data,r);
        
        % performing denoising
        disp('finished nearest neighbor search')
        for i=1:length(Data)
            if length(Index{i,1})<k
                Neighbors = NeighborhoodKNN( Data , i , k );
            else
                Neighbors = Data(Index{i,1},:) ;
            end
            
            % Local PCA
            [EigVec, dim, er] = LocalPCA(Neighbors) ;
            Mean = mean(Neighbors,1);
            if er>Thr
                %             DeltaData = Attraction_MBMS(Data(i,:),Mean,EigVec(:,1:dim));
                DeltaData = Attraction_MBMS_soft(Data(i,:),Neighbors,EigVec(:,1:dim), twosigmatwo);%abst
                Data_New(i,:) = Data(i,:) + DeltaData ;
            else
                Data_New(i,:) = Data(i,:) ;
            end
            %
        end
%     end
    Data = Data_New;%mean(Data_New, 3);
    save(['output_r' num2str(r) '_sigma' num2str(sigma) '_iter' num2str(iter) '.mat'],'Data');
    
end
end

function Delta = Attraction_MBMS_soft(x,Neighbors,EigVec, twosigmatwo)%abst
Kernel = exp(-vecnorm(x - Neighbors, 2, 2).^2 / twosigmatwo);%abst
Mu = sum(Kernel.* Neighbors, 1) / sum(Kernel);
Delta = (Mu-x)*(eye(size(EigVec,1))-EigVec*EigVec');
end

function [EigVec, dim, Error] = LocalPCA(X)
[~, D] = size(X) ;
Mean = mean(X,1);
[~,s,EigVec] = svd(X - Mean,'econ');
EigVal = diag(s).^2 ./ sum(diag(s).^2,1) ;
S = [EigVal(1:D-1) - EigVal(2:D) ; EigVal(D)] ;
S(2:D) = (2:D)'.*S(2:D);%%abst
[~,dim] = max(S) ;
Error = sum(EigVal(dim+1:end)) ;
end

function Neighbors = NeighborhoodKNN( Data , CurrentPos , k )
Data2 = Data(sum(abs(Data-Data(CurrentPos,:)),2)<10,:);
Index = knnsearch(Data2,Data(CurrentPos,:),'K',k);
Neighbors = Data2(Index,:);
end

