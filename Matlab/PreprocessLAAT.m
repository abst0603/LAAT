function [pstruct] = PreprocessLAAT(Data,varargin)
%PreprocessLAAT This function compute neighbor points for each point and then compute
%   eigenvalues and eigenvectors in this neighborhood. Note that if there
%   are not enough points in the neighborhood of a specific point the point
%   will be omitted from the dataset.
%   Data : the complete dataset
%   'radius' : the radius in which the code search for neighbors
%   'threshold' : the mininmum number of points which is needed for
%   computing. If there are less points than the threshold in the
%   neighborhood the point will be omitted.
%   pstruct : the output structure which contain the final data, neighbors
%   of each point, eigen value and eigen vectors and the information about
%   the preprocessing
if nargin > 5
    error('PreprocessLAAT:TooManyInputs', ...
        'requires at most 5 optional inputs');
end
if nargin == 2 || nargin == 4
    error('Please specify a value for the optional inputs');
end
% defult values for threshold and radius
pstruct.threshold = 5;
pstruct.radius = 0.1;
% specify the value based on options
if(~isempty(varargin))
    for i = 1:2:size(varargin,2)
        if(strcmp(varargin{1,i},'Threshold'))
            pstruct.threshold = varargin{1,i+1};
        elseif(strcmp(varargin{1,i},'Radius'))
            pstruct.radius = varargin{1,i+1};
        else
            error('It is not a known option!')
        end
    end
end

% Search the range and dalate the points which don't have enough
% neighbors
[idx,~] = rangesearch(Data,Data,pstruct.radius);
c = 1;
for i = 1:size(Data,1)
    if(size(idx{i},2) < pstruct.threshold)
        rm(c) = i;
        c = c+1;
    end
end
if c~= 1
    Data(rm,:) = [];
end
clear c; clear rm;
% ind and D must be computed again since some points are removed and the
% former data are not true anymore.
flag1 = 0;
for i = 1:10
    [idx,~] = rangesearch(Data,Data,pstruct.radius);
    c = 1;
    for j = 1:size(Data,1)
        if(size(idx{j},2) < pstruct.threshold)
            rm(c) = j;
            c = c + 1;
        end
    end
    if c~= 1
        Data(rm,:) = [];
        clear rm;
    else
        flag1 = 1;
        break;
    end
    warning('the value which is determined for the "thereshold" is very big. The algorithm may not converge!')
end
if(flag1~=1)
    error('the algorithm cannot converge due to the value of thereshold!');
end
EigVec = zeros(size(Data,2),size(Data,2),size(Data,1));
EigVal = zeros(size(Data,2),size(Data,1));
for i=1:size(Data,1)
    signmat = ones(size(Data,2));
    X = Data(idx{i,1},:);
    X = X -mean(X,1);
    [~,s,v] = svd(X,'econ');
    signmat(:,v(1,:)<0) = -1; %% this line make all the eigenvectors in the same direction(remember each vector has two direction, one positive and one negative)
    v = v .* signmat;
    EigVec(:,:,i) = v;
    EigVal(:,i) = diag(s).^2;
end
pstruct.data = Data;
for i = 1:size(Data,1)
    idx{i,1}(1) = [];
end
pstruct.indices = idx;
pstruct.evalue = EigVal./sum(EigVal,1);
pstruct.evector = EigVec;
sizemat = zeros(size(pstruct.indices,1),1);
for i = 1:size(pstruct.data,1)
    sizemat(i) = size(pstruct.indices{i,1},2);
end
pstruct.median = median(sizemat);
end