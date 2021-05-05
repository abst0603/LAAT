clc
clear all
numofpoints = 3000;%how much points on the manifold
r = linspace(0.5,2.5,numofpoints);%medial axis of 2d manifold
randvalue = 0.5 * r .* (rand(1,numofpoints)-0.5);% increase the width of manifold
r = r + randvalue;
tetha = linspace(0,pi,numofpoints);% the curvature is from 0 to pi
x = r.*cos(tetha);
y = r.*sin(tetha);
z = linspace(0,2.5,numofpoints) + rand(1,numofpoints)*0.2; % uniform noise in height too
figure,plot3(x,y,z,'b.');
axis equal

noisepoints = rand(6000,3);
noisepoints(:,1) = noisepoints(:,1) * 5 - 3.5;
noisepoints(:,2) = noisepoints(:,2) * 4 - 1;
noisepoints(:,3) = noisepoints(:,3) * 3.5 - 0.5;

dataset = [[x',y',z'];noisepoints];
figure,plot3(dataset(:,1),dataset(:,2),dataset(:,3),'b.');
axis equal
%dlmwrite('swissroll.csv',dataset,'precision',7);

tetha2 = linspace(pi,2*pi,numofpoints);% the curvature is from pi to 0
x2 = r.*cos(tetha2)-2;
y2 = r.*sin(tetha2)+1.5;
z2 = linspace(0,2.5,numofpoints) + rand(1,numofpoints)*0.2; % uniform noise in height too
dataset = [dataset;[x2',y2',z2']];
figure,plot3(dataset(:,1),dataset(:,2),dataset(:,3),'b.');
axis equal