
fout = fopen('RetinexFilt9x9.h','wb');
fprintf(fout, 'double H1[9][9] = {\n');
cid = 1; for rid = 1:9, fprintf(fout','{%f,%f,%f,%f,%f,%f,%f,%f,%f},\n',H(rid,1,cid),H(rid,2,cid),H(rid,3,cid),H(rid,4,cid),H(rid,5,cid),H(rid,6,cid),H(rid,7,cid),H(rid,8,cid),H(rid,9,cid)); end
fprintf(fout, '};\n');
fprintf(fout, 'double H2[9][9] = {\n');
cid = 2; for rid = 1:9, fprintf(fout','{%f,%f,%f,%f,%f,%f,%f,%f,%f},\n',H(rid,1,cid),H(rid,2,cid),H(rid,3,cid),H(rid,4,cid),H(rid,5,cid),H(rid,6,cid),H(rid,7,cid),H(rid,8,cid),H(rid,9,cid)); end
fprintf(fout, '};\n');
fprintf(fout, 'double H3[9][9] = {\n');
cid = 3; for rid = 1:9, fprintf(fout','{%f,%f,%f,%f,%f,%f,%f,%f,%f},\n',H(rid,1,cid),H(rid,2,cid),H(rid,3,cid),H(rid,4,cid),H(rid,5,cid),H(rid,6,cid),H(rid,7,cid),H(rid,8,cid),H(rid,9,cid)); end
fprintf(fout, '};\n');
fclose(fout)