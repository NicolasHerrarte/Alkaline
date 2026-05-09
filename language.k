proc add(arr_one: int[], arr_two: int[]) -> int[]{
    init int[] arr_ret <- assign int[5];
    for(init int i <- 0;i < 5;i = i + 1){
        arr_ret[i] = arr_one[i] + arr_two[i];
    }

    return arr_ret;
}

init int[] arr_arg_one <- assign int[5];
init int[] arr_arg_two <- assign int[5];

for(init int i <- 0;i < 5;i = i + 1){
    arr_arg_one[i] = i;
    arr_arg_two[i] = (i * i);
}

init int[] final <- add(arr_arg_one, arr_arg_two);

for(init int i <- 0;i < 5;i = i + 1){
    print(final[i]);
}

init int negative_test <- -final[2];
print(negative_test);
print(5-3);