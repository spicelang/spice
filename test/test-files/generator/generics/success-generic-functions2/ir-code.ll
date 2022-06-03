; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Data: %d\0A\00", align 1
@numberList1 = constant [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7]
@numberList2 = constant [4 x i64] [i64 10, i64 12, i64 14, i64 16]
@1 = private unnamed_addr constant [17 x i8] c"Results: %d, %d\0A\00", align 1

; Function Attrs: nounwind
define internal i32 @"_f_sumNumbers_long[]_int"(i64* %0, i32 %1) #0 {
entry:
  %numberArray = alloca i64*, align 8
  %arrayLength = alloca i32, align 4
  %result = alloca i32, align 4
  %2 = alloca i32, align 4
  %i = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  %5 = alloca i32, align 4
  store i64* %0, i64** %numberArray, align 8
  store i32 %1, i32* %arrayLength, align 4
  store i32 0, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  store i32 %6, i32* %result, align 4
  store i32 0, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  store i32 %7, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post, %entry
  %8 = load i32, i32* %i, align 4
  %9 = load i32, i32* %arrayLength, align 4
  %10 = icmp slt i32 %8, %9
  store i1 %10, i1* %4, align 1
  %11 = load i1, i1* %4, align 1
  br i1 %11, label %for, label %for.end

for:                                              ; preds = %for.cond
  %12 = load i64*, i64** %numberArray, align 8
  %13 = load i32, i32* %i, align 4
  %14 = getelementptr inbounds i64, i64* %12, i32 %13
  %15 = load i64, i64* %14, align 8
  %16 = load i32, i32* %result, align 4
  %17 = trunc i64 %15 to i32
  %18 = add i32 %16, %17
  store i32 %18, i32* %result, align 4
  br label %for.post

for.post:                                         ; preds = %for
  %19 = load i32, i32* %i, align 4
  %20 = add i32 %19, 1
  store i32 %20, i32* %i, align 4
  store i32 %19, i32* %5, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %21 = load i32, i32* %result, align 4
  ret i32 %21
}

; Function Attrs: nounwind
define internal i32 @"_f_sumNumbers_short[]_int"(i16* %0, i32 %1) #0 {
entry:
  %numberArray = alloca i16*, align 8
  %arrayLength = alloca i32, align 4
  %result = alloca i32, align 4
  %2 = alloca i32, align 4
  %i = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  %5 = alloca i32, align 4
  store i16* %0, i16** %numberArray, align 8
  store i32 %1, i32* %arrayLength, align 4
  store i32 0, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  store i32 %6, i32* %result, align 4
  store i32 0, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  store i32 %7, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post, %entry
  %8 = load i32, i32* %i, align 4
  %9 = load i32, i32* %arrayLength, align 4
  %10 = icmp slt i32 %8, %9
  store i1 %10, i1* %4, align 1
  %11 = load i1, i1* %4, align 1
  br i1 %11, label %for, label %for.end

for:                                              ; preds = %for.cond
  %12 = load i16*, i16** %numberArray, align 8
  %13 = load i32, i32* %i, align 4
  %14 = getelementptr inbounds i16, i16* %12, i32 %13
  %15 = load i16, i16* %14, align 2
  %16 = load i32, i32* %result, align 4
  %17 = sext i16 %15 to i32
  %18 = add i32 %16, %17
  store i32 %18, i32* %result, align 4
  br label %for.post

for.post:                                         ; preds = %for
  %19 = load i32, i32* %i, align 4
  %20 = add i32 %19, 1
  store i32 %20, i32* %i, align 4
  store i32 %19, i32* %5, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %21 = load i32, i32* %result, align 4
  ret i32 %21
}

; Function Attrs: nounwind
define internal void @"_p_printData_int_int[]"(i32 %0, [2 x i32] %1) #0 {
entry:
  %arrayLength = alloca i32, align 4
  %list = alloca [2 x i32], align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  %4 = alloca i32, align 4
  store i32 %0, i32* %arrayLength, align 4
  store [2 x i32] %1, [2 x i32]* %list, align 4
  store i32 0, i32* %2, align 4
  %5 = load i32, i32* %2, align 4
  store i32 %5, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post, %entry
  %6 = load i32, i32* %i, align 4
  %7 = load i32, i32* %arrayLength, align 4
  %8 = icmp slt i32 %6, %7
  store i1 %8, i1* %3, align 1
  %9 = load i1, i1* %3, align 1
  br i1 %9, label %for, label %for.end

for:                                              ; preds = %for.cond
  %10 = load [2 x i32], [2 x i32]* %list, align 4
  %11 = load i32, i32* %i, align 4
  %12 = getelementptr inbounds [2 x i32], [2 x i32]* %list, i32 0, i32 %11
  %13 = load i32, i32* %12, align 4
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %13)
  br label %for.post

for.post:                                         ; preds = %for
  %15 = load i32, i32* %i, align 4
  %16 = add i32 %15, 1
  store i32 %16, i32* %i, align 4
  store i32 %15, i32* %4, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %numberList1 = alloca [7 x i16], align 2
  %0 = alloca i16, align 2
  %1 = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  %5 = alloca i16, align 2
  %6 = alloca i16, align 2
  %result1 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %numberList2 = alloca [4 x i64], align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i64, align 8
  %result2 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %resultList = alloca [2 x i32], align 4
  %result21 = alloca [2 x i32], align 4
  %15 = alloca i32, align 4
  %16 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i16 1, i16* %0, align 2
  store i16 2, i16* %1, align 2
  store i16 3, i16* %2, align 2
  store i16 4, i16* %3, align 2
  store i16 5, i16* %4, align 2
  store i16 6, i16* %5, align 2
  store i16 7, i16* %6, align 2
  %17 = load [7 x i16], [7 x i16]* @numberList1, align 2
  store [7 x i16] %17, [7 x i16]* %numberList1, align 2
  %18 = getelementptr inbounds [7 x i16], [7 x i16]* %numberList1, i32 0, i32 0
  %19 = load [7 x i16], [7 x i16]* %numberList1, align 2
  store i32 7, i32* %7, align 4
  %20 = load i32, i32* %7, align 4
  %21 = call i32 @"_f_sumNumbers_short[]_int"(i16* %18, i32 %20)
  store i32 %21, i32* %8, align 4
  %22 = load i32, i32* %8, align 4
  store i32 %22, i32* %result1, align 4
  store i64 10, i64* %9, align 8
  store i64 12, i64* %10, align 8
  store i64 14, i64* %11, align 8
  store i64 16, i64* %12, align 8
  %23 = load [4 x i64], [4 x i64]* @numberList2, align 8
  store [4 x i64] %23, [4 x i64]* %numberList2, align 8
  %24 = getelementptr inbounds [4 x i64], [4 x i64]* %numberList2, i32 0, i32 0
  %25 = load [4 x i64], [4 x i64]* %numberList2, align 8
  store i32 4, i32* %13, align 4
  %26 = load i32, i32* %13, align 4
  %27 = call i32 @"_f_sumNumbers_long[]_int"(i64* %24, i32 %26)
  store i32 %27, i32* %14, align 4
  %28 = load i32, i32* %14, align 4
  store i32 %28, i32* %result2, align 4
  %29 = load i32, i32* %result1, align 4
  %30 = getelementptr inbounds [2 x i32], [2 x i32]* %result21, i32 0, i32 0
  store i32 %29, i32* %30, align 4
  %31 = load i32, i32* %result2, align 4
  %32 = getelementptr inbounds [2 x i32], [2 x i32]* %result21, i32 0, i32 1
  store i32 %31, i32* %32, align 4
  %33 = load [2 x i32], [2 x i32]* %result21, align 4
  store [2 x i32] %33, [2 x i32]* %resultList, align 4
  %34 = load [2 x i32], [2 x i32]* %resultList, align 4
  store i32 2, i32* %15, align 4
  %35 = load i32, i32* %15, align 4
  %36 = load [2 x i32], [2 x i32]* %resultList, align 4
  call void @"_p_printData_int_int[]"(i32 %35, [2 x i32] %36)
  store i1 true, i1* %16, align 1
  %37 = load i32, i32* %result1, align 4
  %38 = load i32, i32* %result2, align 4
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %37, i32 %38)
  %40 = load i32, i32* %result, align 4
  ret i32 %40
}

attributes #0 = { nounwind }
