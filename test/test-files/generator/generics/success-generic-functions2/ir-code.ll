; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Data: %d\0A\00", align 1
@numberList1 = constant [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7]
@numberList2 = constant [4 x i64] [i64 10, i64 12, i64 14, i64 16]
@1 = private unnamed_addr constant [17 x i8] c"Results: %d, %d\0A\00", align 1

define internal i32 @_f__void__sumNumbers__longarray_int(i64* %0, i32 %1) {
entry.l4:
  %numberArray = alloca i64*, align 8
  %arrayLength = alloca i32, align 4
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  store i64* %0, i64** %numberArray, align 8
  store i32 %1, i32* %arrayLength, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond.l6

for.l6:                                           ; preds = %for.cond.l6
  %4 = load i64*, i64** %numberArray, align 8
  %5 = load i32, i32* %i, align 4
  %6 = getelementptr inbounds i64, i64* %4, i32 %5
  %7 = load i64, i64* %6, align 8
  %8 = load i32, i32* %result, align 4
  %9 = trunc i64 %7 to i32
  %10 = add i32 %8, %9
  store i32 %10, i32* %result, align 4
  br label %for.inc.l6

for.inc.l6:                                       ; preds = %for.l6
  %11 = load i32, i32* %i, align 4
  %12 = add i32 %11, 1
  store i32 %12, i32* %i, align 4
  store i32 %11, i32* %2, align 4
  br label %for.cond.l6

for.cond.l6:                                      ; preds = %for.inc.l6, %entry.l4
  %13 = load i32, i32* %i, align 4
  %14 = load i32, i32* %arrayLength, align 4
  %15 = icmp slt i32 %13, %14
  store i1 %15, i1* %3, align 1
  %16 = load i1, i1* %3, align 1
  br i1 %16, label %for.l6, label %for.end.l6

for.end.l6:                                       ; preds = %for.cond.l6
  %17 = load i32, i32* %result, align 4
  ret i32 %17
}

define internal i32 @_f__void__sumNumbers__shortarray_int(i16* %0, i32 %1) {
entry.l4:
  %numberArray = alloca i16*, align 8
  %arrayLength = alloca i32, align 4
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  store i16* %0, i16** %numberArray, align 8
  store i32 %1, i32* %arrayLength, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond.l6

for.l6:                                           ; preds = %for.cond.l6
  %4 = load i16*, i16** %numberArray, align 8
  %5 = load i32, i32* %i, align 4
  %6 = getelementptr inbounds i16, i16* %4, i32 %5
  %7 = load i16, i16* %6, align 2
  %8 = load i32, i32* %result, align 4
  %9 = sext i16 %7 to i32
  %10 = add i32 %8, %9
  store i32 %10, i32* %result, align 4
  br label %for.inc.l6

for.inc.l6:                                       ; preds = %for.l6
  %11 = load i32, i32* %i, align 4
  %12 = add i32 %11, 1
  store i32 %12, i32* %i, align 4
  store i32 %11, i32* %2, align 4
  br label %for.cond.l6

for.cond.l6:                                      ; preds = %for.inc.l6, %entry.l4
  %13 = load i32, i32* %i, align 4
  %14 = load i32, i32* %arrayLength, align 4
  %15 = icmp slt i32 %13, %14
  store i1 %15, i1* %3, align 1
  %16 = load i1, i1* %3, align 1
  br i1 %16, label %for.l6, label %for.end.l6

for.end.l6:                                       ; preds = %for.cond.l6
  %17 = load i32, i32* %result, align 4
  ret i32 %17
}

define internal void @_p__void__printData__int_intarray(i32 %0, [2 x i32] %1) {
entry.l11:
  %arrayLength = alloca i32, align 4
  %list = alloca [2 x i32], align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  store i32 %0, i32* %arrayLength, align 4
  store [2 x i32] %1, [2 x i32]* %list, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond.l12

for.l12:                                          ; preds = %for.cond.l12
  %4 = load [2 x i32], [2 x i32]* %list, align 4
  %5 = load i32, i32* %i, align 4
  %6 = getelementptr inbounds [2 x i32], [2 x i32]* %list, i32 0, i32 %5
  %7 = load i32, i32* %6, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %7)
  br label %for.inc.l12

for.inc.l12:                                      ; preds = %for.l12
  %9 = load i32, i32* %i, align 4
  %10 = add i32 %9, 1
  store i32 %10, i32* %i, align 4
  store i32 %9, i32* %2, align 4
  br label %for.cond.l12

for.cond.l12:                                     ; preds = %for.inc.l12, %entry.l11
  %11 = load i32, i32* %i, align 4
  %12 = load i32, i32* %arrayLength, align 4
  %13 = icmp slt i32 %11, %12
  store i1 %13, i1* %3, align 1
  %14 = load i1, i1* %3, align 1
  br i1 %14, label %for.l12, label %for.end.l12

for.end.l12:                                      ; preds = %for.cond.l12
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry.l17:
  %result = alloca i32, align 4
  %numberList1 = alloca [7 x i16], align 2
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %numberList2 = alloca [4 x i64], align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %resultList = alloca [2 x i32], align 4
  %4 = alloca i32, align 4
  %5 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7], [7 x i16]* %numberList1, align 2
  %6 = getelementptr inbounds [7 x i16], [7 x i16]* %numberList1, i32 0, i32 0
  %7 = load [7 x i16], [7 x i16]* %numberList1, align 2
  store i32 7, i32* %0, align 4
  %8 = load i32, i32* %0, align 4
  %9 = call i32 @_f__void__sumNumbers__shortarray_int(i16* %6, i32 %8)
  store i32 %9, i32* %1, align 4
  store [4 x i64] [i64 10, i64 12, i64 14, i64 16], [4 x i64]* %numberList2, align 8
  %10 = getelementptr inbounds [4 x i64], [4 x i64]* %numberList2, i32 0, i32 0
  %11 = load [4 x i64], [4 x i64]* %numberList2, align 8
  store i32 4, i32* %2, align 4
  %12 = load i32, i32* %2, align 4
  %13 = call i32 @_f__void__sumNumbers__longarray_int(i64* %10, i32 %12)
  store i32 %13, i32* %3, align 4
  %14 = load i32, i32* %1, align 4
  %15 = load i32, i32* %3, align 4
  %16 = getelementptr inbounds [2 x i32], [2 x i32]* %resultList, i32 0, i32 0
  store i32 %14, i32* %16, align 4
  %17 = getelementptr inbounds [2 x i32], [2 x i32]* %resultList, i32 0, i32 1
  store i32 %15, i32* %17, align 4
  %18 = load [2 x i32], [2 x i32]* %resultList, align 4
  store i32 2, i32* %4, align 4
  %19 = load i32, i32* %4, align 4
  %20 = load [2 x i32], [2 x i32]* %resultList, align 4
  call void @_p__void__printData__int_intarray(i32 %19, [2 x i32] %20)
  store i1 true, i1* %5, align 1
  %21 = load i32, i32* %1, align 4
  %22 = load i32, i32* %3, align 4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %21, i32 %22)
  %24 = load i32, i32* %result, align 4
  ret i32 %24
}
