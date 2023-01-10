; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Data: %d\0A\00", align 1
@anon.array.0 = private unnamed_addr constant [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7]
@anon.array.1 = private unnamed_addr constant [4 x i64] [i64 10, i64 12, i64 14, i64 16]
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Results: %d, %d\0A\00", align 1

define private i32 @_f__void__int__sumNumbers__shortarray_int(ptr %0, i32 %1) {
  %result = alloca i32, align 4
  %numberArray = alloca ptr, align 8
  %arrayLength = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %0, ptr %numberArray, align 8
  store i32 %1, ptr %arrayLength, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L6

for.head.L6:                                      ; preds = %for.tail.L6, %2
  %3 = load i32, ptr %arrayLength, align 4
  %4 = load i32, ptr %i, align 4
  %5 = icmp slt i32 %4, %3
  br i1 %5, label %for.body.L6, label %for.exit.L6

for.body.L6:                                      ; preds = %for.head.L6
  %6 = load i32, ptr %i, align 4
  %7 = load ptr, ptr %numberArray, align 8
  %8 = getelementptr inbounds i16, ptr %7, i32 %6
  %9 = load i16, ptr %8, align 2
  %10 = sext i16 %9 to i32
  %11 = load i32, ptr %result, align 4
  %12 = add i32 %11, %10
  store i32 %12, ptr %result, align 4
  br label %for.tail.L6

for.tail.L6:                                      ; preds = %for.body.L6
  %13 = load i32, ptr %i, align 4
  %14 = add i32 %13, 1
  store i32 %14, ptr %i, align 4
  br label %for.head.L6

for.exit.L6:                                      ; preds = %for.head.L6
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

define private i32 @_f__void__int__sumNumbers__longarray_int(ptr %0, i32 %1) {
  %result = alloca i32, align 4
  %numberArray = alloca ptr, align 8
  %arrayLength = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %0, ptr %numberArray, align 8
  store i32 %1, ptr %arrayLength, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L6

for.head.L6:                                      ; preds = %for.tail.L6, %2
  %3 = load i32, ptr %arrayLength, align 4
  %4 = load i32, ptr %i, align 4
  %5 = icmp slt i32 %4, %3
  br i1 %5, label %for.body.L6, label %for.exit.L6

for.body.L6:                                      ; preds = %for.head.L6
  %6 = load i32, ptr %i, align 4
  %7 = load ptr, ptr %numberArray, align 8
  %8 = getelementptr inbounds i64, ptr %7, i32 %6
  %9 = load i64, ptr %8, align 8
  %10 = trunc i64 %9 to i32
  %11 = load i32, ptr %result, align 4
  %12 = add i32 %11, %10
  store i32 %12, ptr %result, align 4
  br label %for.tail.L6

for.tail.L6:                                      ; preds = %for.body.L6
  %13 = load i32, ptr %i, align 4
  %14 = add i32 %13, 1
  store i32 %14, ptr %i, align 4
  br label %for.head.L6

for.exit.L6:                                      ; preds = %for.head.L6
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

define private void @_p__void__void__printData__int_intarray(i32 %0, [2 x i32] %1) {
  %arrayLength = alloca i32, align 4
  %list = alloca [2 x i32], align 4
  %i = alloca i32, align 4
  store i32 %0, ptr %arrayLength, align 4
  store [2 x i32] %1, ptr %list, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L12

for.head.L12:                                     ; preds = %for.tail.L12, %2
  %3 = load i32, ptr %arrayLength, align 4
  %4 = load i32, ptr %i, align 4
  %5 = icmp slt i32 %4, %3
  br i1 %5, label %for.body.L12, label %for.exit.L12

for.body.L12:                                     ; preds = %for.head.L12
  %6 = load i32, ptr %i, align 4
  %7 = getelementptr inbounds [2 x i32], ptr %list, i32 0, i32 %6
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %8)
  br label %for.tail.L12

for.tail.L12:                                     ; preds = %for.body.L12
  %10 = load i32, ptr %i, align 4
  %11 = add i32 %10, 1
  store i32 %11, ptr %i, align 4
  br label %for.head.L12

for.exit.L12:                                     ; preds = %for.head.L12
  ret void
}

declare i32 @printf(ptr noundef, ...)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca [7 x i16], align 2
  %numberList1 = alloca [7 x i16], align 2
  %result1 = alloca i32, align 4
  %2 = alloca [4 x i64], align 8
  %numberList2 = alloca [4 x i64], align 8
  %result2 = alloca i32, align 4
  %3 = alloca [2 x i32], align 4
  %resultList = alloca [2 x i32], align 4
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 14, i1 false)
  store [7 x i16] [i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7], ptr %numberList1, align 2
  %4 = getelementptr inbounds [7 x i16], ptr %numberList1, i32 0, i32 0
  %5 = call i32 @_f__void__int__sumNumbers__shortarray_int(ptr %4, i32 7)
  store i32 %5, ptr %result1, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %2, ptr @anon.array.1, i64 32, i1 false)
  store [4 x i64] [i64 10, i64 12, i64 14, i64 16], ptr %numberList2, align 8
  %6 = getelementptr inbounds [4 x i64], ptr %numberList2, i32 0, i32 0
  %7 = call i32 @_f__void__int__sumNumbers__longarray_int(ptr %6, i32 4)
  store i32 %7, ptr %result2, align 4
  %8 = getelementptr inbounds [2 x i32], ptr %3, i32 0, i32 0
  %9 = load i32, ptr %result1, align 4
  store i32 %9, ptr %8, align 4
  %10 = load i32, ptr %result2, align 4
  %11 = getelementptr inbounds i32, ptr %8, i32 1
  store i32 %10, ptr %11, align 4
  %12 = load [2 x i32], ptr %3, align 4
  store [2 x i32] %12, ptr %resultList, align 4
  %13 = load [2 x i32], ptr %resultList, align 4
  call void @_p__void__void__printData__int_intarray(i32 2, [2 x i32] %13)
  %14 = load i32, ptr %result1, align 4
  %15 = load i32, ptr %result2, align 4
  %16 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %14, i32 %15)
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

; Function Attrs: argmemonly nocallback nofree nounwind willreturn
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { argmemonly nocallback nofree nounwind willreturn }
