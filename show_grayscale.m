m1 = load('E2projective.txt');
m2 = load('E2receptive.txt');
for i = 1:100
    for j = 1:10
        t1(i,j,:) = m1((i-1)*10 + (j-1) + 1 : (i-1)*10 + j, :);
        t2(i,j,:) = m2((i-1)*10 + (j-1) + 1 : (i-1)*10 + j, :);
    end
end

max1 = max(max(max(t1))); max2 = max(max(max(t2)));
for i = 1:109
    for j = 1:109
        p1(i,j)=4; p2(i,j)=4;
    end
end

for i = 1:100
    for j = 1:100
        p1(i + int8(floor((i-1)/10)), j + int8(floor((j-1)/10))) = t1(int8(floor((i-1)/10)*10) + int8(floor((j-1)/10)) + 1, i - int8(floor((i-1)/10)*10), j - int8(floor((j-1) / 10)*10));
        p2(i + int8(floor((i-1)/10)), j + int8(floor((j-1)/10))) = t2(int8(floor((i-1)/10)*10) + int8(floor((j-1)/10)) + 1, i - int8(floor((i-1)/10)*10), j - int8(floor((j-1) / 10)*10));
    end
end

p1 = mat2gray(p1, [0, max1]);
p2 = mat2gray(p2, [0, max2]);
subplot(1,2,1);
imshow(p1);
subplot(1,2,2);
imshow(p2);