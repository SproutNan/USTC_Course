function im2 = RBFImageWarp(im, psrc, pdst)

% input: im, psrc, pdst

[h, w, dim] = size(im);
[n, ~] = size(psrc);
im2 = zeros(h, w, dim, 'uint8') + 255;
psrc(:,[1,2]) = psrc(:,[2,1]);
pdst(:,[1,2]) = pdst(:,[2,1]);
d = 30000;

dists = pdist2(pdst, pdst).^2;
bp_map = 1 ./ (dists + d);
A = bp_map \ (psrc - pdst);

src = transpose(meshgrid(1:h,1:w));
dst = meshgrid(1:w,1:h);

rever_f = zeros(h, w, 2);
for k = 1:n
    q = 1 ./ ((src - pdst(k,1)).^2 + (dst - pdst(k,2)).^2 + d);
    rever_f(:,:,1) = rever_f(:,:,1) + A(k,1) * q;
    rever_f(:,:,2) = rever_f(:,:,2) + A(k,2) * q;
end
rever_f = round(rever_f + cat(3, src, dst));

for i = 1:h
    for j = 1:w
        x = rever_f(i, j, 1);
        y = rever_f(i, j, 2);
        if x < 1 || x > h || y < 1 || y > w
            im2(i,j,:) = [0,0,0];
        else
            im2(i,j,:) = im(x, y, :);
        end
    end
end