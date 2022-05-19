beta = 10;
for i = 1:size(pstruct.data,1)
    kNN{i} = pstruct.data(pstruct.indices{i,1},:) - pstruct.data(i,:);
    kNN{i} = kNN{i}./vecnorm(kNN{i},2,2);
    V{i} = (abs(kNN{i}*pstruct.evector(:,:,i))./repmat(sum(abs(kNN{i}*pstruct.evector(:,:,i)),2),1,size(kNN{i},2))) * pstruct.evalue(:,i);
    P{i} = exp(beta * V{i}) / sum(exp(beta * V{i}));
end

transmat = sparse(size(pstruct.data,1),size(pstruct.data,1));
for i = 1:size(pstruct.data,1)
    for j = 1:size(pstruct.indices{i},2)
        transmat(i,pstruct.indices{i}(j)) = P{i}(j);
    end
end

datasize = size(pstruct.data,1);
q1 = [0.5*ones(round(datasize/2),1);ones(datasize - round(datasize/2),1)]';
q0 = ones(size(pstruct.data,1),1)';
iter = 0;
while(sum(abs(q1-q0))>0.1)
    q0 = q1;
    q1 = q0 * transmat;
    iter = iter + 1;
end