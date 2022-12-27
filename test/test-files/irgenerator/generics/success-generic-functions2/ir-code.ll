; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Data: %d\0A\00", align 1
@numberList1 = global [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7]
@numberList2 = global [4 x i64] [i64 10, i64 12, i64 14, i64 16]
@1 = private unnamed_addr constant [17 x i8] c"Results: %d, %d\0A\00", align 1

define internal i32 @_f__void__int__sumNumbers__longarray_int(ptr %0, i32 %1) {
entry.l4:
  %numberArray = alloca ptr, align 8
  %arrayLength = alloca i32, align 4
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  store ptr %0, ptr %numberArray, align 8
  store i32 %1, ptr %arrayLength, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l6

for.l6:                                           ; preds = %for.cond.l6
  %4 = load i32, ptr %i, align 4
  %5 = load ptr, ptr %numberArray, align 8
  %6 = getelementptr inbounds i64, ptr %5, i32 %4
  %7 = load i64, ptr %6, align 8
  %8 = load i32, ptr %result, align 4
  %9 = trunc i64 %7 to i32
  %10 = add i32 %8, %9
  store i32 %10, ptr %result, align 4
  br label %for.inc.l6

for.inc.l6:                                       ; preds = %for.l6
  %11 = load i32, ptr %i, align 4
  %12 = add i32 %11, 1
  store i32 %12, ptr %i, align 4
  store i32 %11, ptr %2, align 4
  br label %for.cond.l6

for.cond.l6:                                      ; preds = %for.inc.l6, %entry.l4
  %13 = load i32, ptr %i, align 4
  %14 = load i32, ptr %arrayLength, align 4
  %15 = icmp slt i32 %13, %14
  store i1 %15, ptr %3, align 1
  %16 = load i1, ptr %3, align 1
  br i1 %16, label %for.l6, label %for.end.l6

for.end.l6:                                       ; preds = %for.cond.l6
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

define internal i32 @_f__void__int__sumNumbers__shortarray_int(ptr %0, i32 %1) {
entry.l4:
  %numberArray = alloca ptr, align 8
  %arrayLength = alloca i32, align 4
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  store ptr %0, ptr %numberArray, align 8
  store i32 %1, ptr %arrayLength, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l6

for.l6:                                           ; preds = %for.cond.l6
  %4 = load i32, ptr %i, align 4
  %5 = load ptr, ptr %numberArray, align 8
  %6 = getelementptr inbounds i16, ptr %5, i32 %4
  %7 = load i16, ptr %6, align 2
  %8 = load i32, ptr %result, align 4
  %9 = sext i16 %7 to i32
  %10 = add i32 %8, %9
  store i32 %10, ptr %result, align 4
  br label %for.inc.l6

for.inc.l6:                                       ; preds = %for.l6
  %11 = load i32, ptr %i, align 4
  %12 = add i32 %11, 1
  store i32 %12, ptr %i, align 4
  store i32 %11, ptr %2, align 4
  br label %for.cond.l6

for.cond.l6:                                      ; preds = %for.inc.l6, %entry.l4
  %13 = load i32, ptr %i, align 4
  %14 = load i32, ptr %arrayLength, align 4
  %15 = icmp slt i32 %13, %14
  store i1 %15, ptr %3, align 1
  %16 = load i1, ptr %3, align 1
  br i1 %16, label %for.l6, label %for.end.l6

for.end.l6:                                       ; preds = %for.cond.l6
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

define internal void @_p__void__void__printData__int_intarray(i32 %0, [2 x i32] %1) {
entry.l11:
  %arrayLength = alloca i32, align 4
  %list = alloca [2 x i32], align 4
  %i = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  store i32 %0, ptr %arrayLength, align 4
  store [2 x i32] %1, ptr %list, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l12

for.l12:                                          ; preds = %for.cond.l12
  %4 = load i32, ptr %i, align 4
  %5 = getelementptr inbounds [2 x i32], ptr %list, i32 0, i32 %4
  %6 = load i32, ptr %5, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @0, i32 %6)
  br label %for.inc.l12

for.inc.l12:                                      ; preds = %for.l12
  %8 = load i32, ptr %i, align 4
  %9 = add i32 %8, 1
  store i32 %9, ptr %i, align 4
  store i32 %8, ptr %2, align 4
  br label %for.cond.l12

for.cond.l12:                                     ; preds = %for.inc.l12, %entry.l11
  %10 = load i32, ptr %i, align 4
  %11 = load i32, ptr %arrayLength, align 4
  %12 = icmp slt i32 %10, %11
  store i1 %12, ptr %3, align 1
  %13 = load i1, ptr %3, align 1
  br i1 %13, label %for.l12, label %for.end.l12

for.end.l12:                                      ; preds = %for.cond.l12
  ret void
}

declare i32 @printf(ptr, ...)

define i32 @main() {
entry.l17:
  %result = alloca i32, align 4
  %numberList1 = alloca [7 x i16], align 2
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %result1 = alloca i32, align 4
  %numberList2 = alloca [4 x i64], align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %result2 = alloca i32, align 4
  %4 = alloca [2 x i32], align 4
  %resultList = alloca [2 x i32], align 4
  %5 = alloca i32, align 4
  %6 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  %7 = load [7 x i16], ptr @numberList1, align 2
  store [7 x i16] %7, ptr %numberList1, align 2
  %8 = getelementptr inbounds [7 x i16], ptr %numberList1, i32 0, i32 0
  %9 = load [7 x i16], ptr %numberList1, align 2
  store i32 7, ptr %0, align 4
  %10 = load i32, ptr %0, align 4
  %11 = call i32 @_f__void__int__sumNumbers__shortarray_int(ptr %8, i32 %10)
  store i32 %11, ptr %1, align 4
  %12 = load i32, ptr %1, align 4
  store i32 %12, ptr %result1, align 4
  %13 = load [4 x i64], ptr @numberList2, align 8
  store [4 x i64] %13, ptr %numberList2, align 8
  %14 = getelementptr inbounds [4 x i64], ptr %numberList2, i32 0, i32 0
  %15 = load [4 x i64], ptr %numberList2, align 8
  store i32 4, ptr %2, align 4
  %16 = load i32, ptr %2, align 4
  %17 = call i32 @_f__void__int__sumNumbers__longarray_int(ptr %14, i32 %16)
  store i32 %17, ptr %3, align 4
  %18 = load i32, ptr %3, align 4
  store i32 %18, ptr %result2, align 4
  %19 = load i32, ptr %result1, align 4
  %20 = load i32, ptr %result2, align 4
  %21 = getelementptr inbounds [2 x i32], ptr %4, i32 0, i32 0
  store i32 %19, ptr %21, align 4
  %22 = getelementptr inbounds [2 x i32], ptr %4, i32 0, i32 1
  store i32 %20, ptr %22, align 4
  %23 = load [2 x i32], ptr %4, align 4
  store [2 x i32] %23, ptr %resultList, align 4
  %24 = load [2 x i32], ptr %resultList, align 4
  store i32 2, ptr %5, align 4
  %25 = load i32, ptr %5, align 4
  %26 = load [2 x i32], ptr %resultList, align 4
  call void @_p__void__void__printData__int_intarray(i32 %25, [2 x i32] %26)
  store i1 true, ptr %6, align 1
  %27 = load i32, ptr %result1, align 4
  %28 = load i32, ptr %result2, align 4
  %29 = call i32 (ptr, ...) @printf(ptr @1, i32 %27, i32 %28)
  %30 = load i32, ptr %result, align 4
  ret i32 %30
}