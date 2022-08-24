; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Data: %d\0A\00", align 1
@numberList1 = global [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7]
@numberList2 = global [4 x i64] [i64 10, i64 12, i64 14, i64 16]
@1 = private unnamed_addr constant [17 x i8] c"Results: %d, %d\0A\00", align 1

define internal i32 @_f__void__sumNumbers__longarray_int(ptr %0, i32 %1) {
entry.l4:
  %numberArray = alloca ptr, align 8
  store ptr %0, ptr %numberArray, align 8
  %arrayLength = alloca i32, align 4
  store i32 %1, ptr %arrayLength, align 4
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l6

for.l6:                                           ; preds = %for.cond.l6
  %2 = load i32, ptr %i, align 4
  %3 = load ptr, ptr %numberArray, align 8
  %4 = getelementptr inbounds i64, ptr %3, i32 %2
  %5 = load i64, ptr %4, align 8
  %6 = load i32, ptr %result, align 4
  %7 = trunc i64 %5 to i32
  %8 = add i32 %6, %7
  store i32 %8, ptr %result, align 4
  br label %for.inc.l6

for.inc.l6:                                       ; preds = %for.l6
  %9 = load i32, ptr %i, align 4
  %10 = add i32 %9, 1
  store i32 %10, ptr %i, align 4
  %11 = alloca i32, align 4
  store i32 %9, ptr %11, align 4
  br label %for.cond.l6

for.cond.l6:                                      ; preds = %for.inc.l6, %entry.l4
  %12 = load i32, ptr %i, align 4
  %13 = load i32, ptr %arrayLength, align 4
  %14 = icmp slt i32 %12, %13
  %15 = alloca i1, align 1
  store i1 %14, ptr %15, align 1
  %16 = load i1, ptr %15, align 1
  br i1 %16, label %for.l6, label %for.end.l6

for.end.l6:                                       ; preds = %for.cond.l6
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

define internal i32 @_f__void__sumNumbers__shortarray_int(ptr %0, i32 %1) {
entry.l4:
  %numberArray = alloca ptr, align 8
  store ptr %0, ptr %numberArray, align 8
  %arrayLength = alloca i32, align 4
  store i32 %1, ptr %arrayLength, align 4
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l6

for.l6:                                           ; preds = %for.cond.l6
  %2 = load i32, ptr %i, align 4
  %3 = load ptr, ptr %numberArray, align 8
  %4 = getelementptr inbounds i16, ptr %3, i32 %2
  %5 = load i16, ptr %4, align 2
  %6 = load i32, ptr %result, align 4
  %7 = sext i16 %5 to i32
  %8 = add i32 %6, %7
  store i32 %8, ptr %result, align 4
  br label %for.inc.l6

for.inc.l6:                                       ; preds = %for.l6
  %9 = load i32, ptr %i, align 4
  %10 = add i32 %9, 1
  store i32 %10, ptr %i, align 4
  %11 = alloca i32, align 4
  store i32 %9, ptr %11, align 4
  br label %for.cond.l6

for.cond.l6:                                      ; preds = %for.inc.l6, %entry.l4
  %12 = load i32, ptr %i, align 4
  %13 = load i32, ptr %arrayLength, align 4
  %14 = icmp slt i32 %12, %13
  %15 = alloca i1, align 1
  store i1 %14, ptr %15, align 1
  %16 = load i1, ptr %15, align 1
  br i1 %16, label %for.l6, label %for.end.l6

for.end.l6:                                       ; preds = %for.cond.l6
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

define internal void @_p__void__printData__int_intarray(i32 %0, [2 x i32] %1) {
entry.l11:
  %arrayLength = alloca i32, align 4
  store i32 %0, ptr %arrayLength, align 4
  %list = alloca [2 x i32], align 4
  store [2 x i32] %1, ptr %list, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l12

for.l12:                                          ; preds = %for.cond.l12
  %2 = load i32, ptr %i, align 4
  %3 = getelementptr inbounds [2 x i32], ptr %list, i32 0, i32 %2
  %4 = load i32, ptr %3, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0, i32 %4)
  br label %for.inc.l12

for.inc.l12:                                      ; preds = %for.l12
  %6 = load i32, ptr %i, align 4
  %7 = add i32 %6, 1
  store i32 %7, ptr %i, align 4
  %8 = alloca i32, align 4
  store i32 %6, ptr %8, align 4
  br label %for.cond.l12

for.cond.l12:                                     ; preds = %for.inc.l12, %entry.l11
  %9 = load i32, ptr %i, align 4
  %10 = load i32, ptr %arrayLength, align 4
  %11 = icmp slt i32 %9, %10
  %12 = alloca i1, align 1
  store i1 %11, ptr %12, align 1
  %13 = load i1, ptr %12, align 1
  br i1 %13, label %for.l12, label %for.end.l12

for.end.l12:                                      ; preds = %for.cond.l12
  ret void
}

declare i32 @printf(ptr, ...)

define i32 @main() {
entry.l17:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = load [7 x i16], ptr @numberList1, align 2
  %1 = alloca i32, align 4
  store i32 7, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  %3 = call i32 @_f__void__sumNumbers__shortarray_int(ptr @numberList1, i32 %2)
  %4 = alloca i32, align 4
  store i32 %3, ptr %4, align 4
  %5 = load [4 x i64], ptr @numberList2, align 8
  %6 = alloca i32, align 4
  store i32 4, ptr %6, align 4
  %7 = load i32, ptr %6, align 4
  %8 = call i32 @_f__void__sumNumbers__longarray_int(ptr @numberList2, i32 %7)
  %9 = alloca i32, align 4
  store i32 %8, ptr %9, align 4
  %10 = load i32, ptr %4, align 4
  %11 = load i32, ptr %9, align 4
  %12 = alloca [2 x i32], align 4
  %13 = getelementptr inbounds [2 x i32], ptr %12, i32 0, i32 0
  store i32 %10, ptr %13, align 4
  %14 = getelementptr inbounds [2 x i32], ptr %12, i32 0, i32 1
  store i32 %11, ptr %14, align 4
  %15 = load [2 x i32], ptr %12, align 4
  %16 = alloca i32, align 4
  store i32 2, ptr %16, align 4
  %17 = load i32, ptr %16, align 4
  %18 = load [2 x i32], ptr %12, align 4
  call void @_p__void__printData__int_intarray(i32 %17, [2 x i32] %18)
  %19 = alloca i1, align 1
  store i1 true, ptr %19, align 1
  %20 = load i32, ptr %4, align 4
  %21 = load i32, ptr %9, align 4
  %22 = call i32 (ptr, ...) @printf(ptr @1, i32 %20, i32 %21)
  %23 = load i32, ptr %result, align 4
  ret i32 %23
}
