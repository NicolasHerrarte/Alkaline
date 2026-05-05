init int[] dim_one <- assign int[5];

dim_one[0] = 11;
dim_one[1] = 22;
dim_one[2] = 33;
dim_one[3] = 44;
dim_one[4] = 55;

init int[][] dim_two <- assign int[5][2];

for(init int i <- 0;i < 5;i = i + 1){
    dim_two[1][i] = i - ((i+1) * 2);
}

for(init int j <- 0;j < 5;j = j + 1){
    print(dim_two[1][j]);
}