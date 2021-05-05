clc
clear all
tmpdata = rand(2000,3);
aa = (tmpdata(:,1)-0.5).^2 + (tmpdata(:,2)-0.5).^2 <= 0.25;
tmpdata = tmpdata(aa,:);
tmpdata = tmpdata(randperm(size(tmpdata,1),1000),:);
dataset = tmpdata;
tmpdata1 = (tmpdata.*[1,1,3])+[3,0,0];
dataset = [dataset;tmpdata1];
tmpdata2 = (tmpdata.*[0.5,0.5,2])+[3,1.5,0];
tmpdata2(:,1:3) = tmpdata2(:,3:-1:1);
dataset = [dataset;tmpdata2];
tmpdata3 = (tmpdata.*[1.5,1.5,4])+[1,1,0];
tmpdata3(:,1:3) = tmpdata3(:,[2,3,1]);
dataset = [dataset;tmpdata3];
tmp4 = rand(5000,3)*4;
dataset = [dataset;tmp4];
plot3(dataset(:,1),dataset(:,2),dataset(:,3),'r.');
axis equal
