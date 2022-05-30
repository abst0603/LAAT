function [pheromone] = LAAT(instruct,varargin)
% Apply LAAT in a neighbourhood.
%   instruct: the output of function PreprocessLAAT.
%	'Plot_data' if it is 1 then it will plot the result after each iteration.
%	if it is 0 it will not plot anything.
%	'Option' importing the option for the algorithm from a '.mat' file. There
%	is a sample for this file in the folder.
if nargin > 3
    error('LAAT:TooManyInputs', ...
        'requires at most 3 optional inputs');
end
% defult values for threshold and radius
flag1 = 0;
% specify the value based on options
if(~isempty(varargin))
    for i = 1:2:size(varargin,2)
        if(strcmp(varargin{1,i},'Option'))
            Option = varargin{1,i+1};
            flag1 = 1;
        else
            error('It is not a known option!')
        end
    end
end

if(~flag1)
    Option.NumberOfAnts = 500 ;
    Option.NumberOfIteration = 20 ;
    Option.NumberOfSteps = 2500 ;
    Option.beta = 10 ;
    Option.kappa = 0.5 ;
    Option.p_release = 0.05;
    Option.EvapRate = 0.1;
else
    load(Option);
end
%% ************* Searching Algorithm
pheromone = ones(size(instruct.data,1),1);
for Loop = 1:Option.NumberOfIteration
    H = zeros(size(instruct.data,1),1);
    Loop
    for Ants = 1:Option.NumberOfAnts
        pheromoneout = AntSearchPCA(Option, instruct, pheromone);
        pheromone = pheromone + pheromoneout;
    end
    pheromone = (1-Option.EvapRate)*pheromone;%%evaporation formula
    pheromone(pheromone<0.01) = 0.000001 ;
    
end

end

function pheromoneout = AntSearchPCA(Option, instruct, pheromone)
Visited = zeros(Option.NumberOfSteps,1);
%% ********* Randomly intialization of Ants
neighborhood_size = 0;
while(neighborhood_size < instruct.median)
    CurrentPos = randi([1 size(instruct.data,1)], 1, 1);
    neighborhood_size = size(instruct.indices{CurrentPos,1},2);
end
Visited(1) = CurrentPos;
pheromoneout = zeros([size(instruct.data,1),1]);
for i = 2:Option.NumberOfSteps
    kNN = instruct.data(instruct.indices{CurrentPos,1},:) - instruct.data(CurrentPos,:);
    if verLessThan('matlab','9.4.0')
        vnorm = ones(size(kNN,1),1);
        for j = 1:size(kNN,1)
            vnorm(j) = norm(kNN(j,:));
        end
        kNN = kNN ./ vnorm ;
    else
        kNN = kNN./vecnorm(kNN,2,2);
    end
    % compute the normalized pheromone
    H = pheromone(instruct.indices{CurrentPos,1}) ;
    H = H/sum(H);
    % compute the normalized effect of neighbour points direction
    V = (abs(kNN*instruct.evector(:,:,CurrentPos))./repmat(sum(abs(kNN*instruct.evector(:,:,CurrentPos)),2),1,size(kNN,2))) * instruct.evalue(:,CurrentPos);
    V = (1-Option.kappa)*H + Option.kappa*V;
    % compute the probability
    P = exp(Option.beta * V) / sum(exp(Option.beta * V));%abst
    clear V;
    pp = cumsum(P)/sum(P);
    % loop in the path is allowed
    itemp = find(pp>rand(1),1);
    tempPos = instruct.indices{CurrentPos,1}(itemp);
    CurrentPos = tempPos;
    Visited(i) = CurrentPos;
    pheromoneout(CurrentPos) = pheromoneout(CurrentPos) + Option.p_release;
end
end