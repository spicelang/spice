; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Data: %d\0A\00", align 1
@numberList1 = constant [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7]
@numberList2 = constant [4 x i64] [i64 10, i64 12, i64 14, i64 16]
@1 = private unnamed_addr constant [17 x i8] c"Results: %d, %d\0A\00", align 1

define internal i32 @_f__void__sumNumbers__longarray_int(i64* %0, i32 %1) {
entry:
  %numberArray = alloca i64*, align 8
  %arrayLength = alloca i32, align 4
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  store i64* %0, i64** %numberArray, align 8
  store i32 %1, i32* %arrayLength, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %2, align 4
  br label %for.cond

for:                                              ; preds = %for.cond
  %5 = load i64*, i64** %numberArray, align 8
  %6 = load i32, i32* %2, align 4
  %7 = getelementptr inbounds i64, i64* %5, i32 %6
  %8 = load i64, i64* %7, align 8
  %9 = load i32, i32* %result, align 4
  %10 = trunc i64 %8 to i32
  %11 = add i32 %9, %10
  store i32 %11, i32* %result, align 4
  br label %for.inc

for.inc:                                          ; preds = %for
  %12 = load i32, i32* %2, align 4
  %13 = add i32 %12, 1
  store i32 %13, i32* %2, align 4
  store i32 %12, i32* %3, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %14 = load i32, i32* %2, align 4
  %15 = load i32, i32* %arrayLength, align 4
  %16 = icmp slt i32 %14, %15
  store i1 %16, i1* %4, align 1
  %17 = load i1, i1* %4, align 1
  br i1 %17, label %for, label %for.end

for.end:                                          ; preds = %for.cond
  %18 = load i32, i32* %result, align 4
  ret i32 %18
}

define internal i32 @_f__void__sumNumbers__shortarray_int(i16* %0, i32 %1) {
entry:
  %numberArray = alloca i16*, align 8
  %arrayLength = alloca i32, align 4
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  store i16* %0, i16** %numberArray, align 8
  store i32 %1, i32* %arrayLength, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %2, align 4
  br label %for.cond

for:                                              ; preds = %for.cond
  %5 = load i16*, i16** %numberArray, align 8
  %6 = load i32, i32* %2, align 4
  %7 = getelementptr inbounds i16, i16* %5, i32 %6
  %8 = load i16, i16* %7, align 2
  %9 = load i32, i32* %result, align 4
  %10 = sext i16 %8 to i32
  %11 = add i32 %9, %10
  store i32 %11, i32* %result, align 4
  br label %for.inc

for.inc:                                          ; preds = %for
  %12 = load i32, i32* %2, align 4
  %13 = add i32 %12, 1
  store i32 %13, i32* %2, align 4
  store i32 %12, i32* %3, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %14 = load i32, i32* %2, align 4
  %15 = load i32, i32* %arrayLength, align 4
  %16 = icmp slt i32 %14, %15
  store i1 %16, i1* %4, align 1
  %17 = load i1, i1* %4, align 1
  br i1 %17, label %for, label %for.end

for.end:                                          ; preds = %for.cond
  %18 = load i32, i32* %result, align 4
  ret i32 %18
}

define internal void @_p__void__printData__int_intarray(i32 %0, [2 x i32] %1) {
entry:
  %arrayLength = alloca i32, align 4
  %list = alloca [2 x i32], align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  store i32 %0, i32* %arrayLength, align 4
  store [2 x i32] %1, [2 x i32]* %list, align 4
  store i32 0, i32* %2, align 4
  br label %for.cond

for:                                              ; preds = %for.cond
  %5 = load [2 x i32], [2 x i32]* %list, align 4
  %6 = load i32, i32* %2, align 4
  %7 = getelementptr inbounds [2 x i32], [2 x i32]* %list, i32 0, i32 %6
  %8 = load i32, i32* %7, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %8)
  br label %for.inc

for.inc:                                          ; preds = %for
  %10 = load i32, i32* %2, align 4
  %11 = add i32 %10, 1
  store i32 %11, i32* %2, align 4
  store i32 %10, i32* %3, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %12 = load i32, i32* %2, align 4
  %13 = load i32, i32* %arrayLength, align 4
  %14 = icmp slt i32 %12, %13
  store i1 %14, i1* %4, align 1
  %15 = load i1, i1* %4, align 1
  br i1 %15, label %for, label %for.end

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
  %numberList11 = alloca [7 x i16], align 2
  %result1 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %numberList2 = alloca [4 x i64], align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i64, align 8
  %numberList22 = alloca [4 x i64], align 8
  %result2 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %resultList = alloca [2 x i32], align 4
  %result23 = alloca [2 x i32], align 4
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
  store [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7], [7 x i16]* %numberList11, align 2
  %17 = getelementptr inbounds [7 x i16], [7 x i16]* %numberList11, i32 0, i32 0
  %18 = load [7 x i16], [7 x i16]* %numberList11, align 2
  store i32 7, i32* %7, align 4
  %19 = load i32, i32* %7, align 4
  %20 = call i32 @_f__void__sumNumbers__shortarray_int(i16* %17, i32 %19)
  store i32 %20, i32* %8, align 4
  store i64 10, i64* %9, align 8
  store i64 12, i64* %10, align 8
  store i64 14, i64* %11, align 8
  store i64 16, i64* %12, align 8
  store [4 x i64] [i64 10, i64 12, i64 14, i64 16], [4 x i64]* %numberList22, align 8
  %21 = getelementptr inbounds [4 x i64], [4 x i64]* %numberList22, i32 0, i32 0
  %22 = load [4 x i64], [4 x i64]* %numberList22, align 8
  store i32 4, i32* %13, align 4
  %23 = load i32, i32* %13, align 4
  %24 = call i32 @_f__void__sumNumbers__longarray_int(i64* %21, i32 %23)
  store i32 %24, i32* %14, align 4
  %25 = load i32, i32* %8, align 4
  %26 = getelementptr inbounds [2 x i32], [2 x i32]* %result23, i32 0, i32 0
  store i32 %25, i32* %26, align 4
  %27 = load i32, i32* %14, align 4
  %28 = getelementptr inbounds [2 x i32], [2 x i32]* %result23, i32 0, i32 1
  store i32 %27, i32* %28, align 4
  %29 = load [2 x i32], [2 x i32]* %result23, align 4
  store i32 2, i32* %15, align 4
  %30 = load i32, i32* %15, align 4
  %31 = load [2 x i32], [2 x i32]* %result23, align 4
  call void @_p__void__printData__int_intarray(i32 %30, [2 x i32] %31)
  store i1 true, i1* %16, align 1
  %32 = load i32, i32* %8, align 4
  %33 = load i32, i32* %14, align 4
  %34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %32, i32 %33)
  %35 = load i32, i32* %result, align 4
  ret i32 %35
}
