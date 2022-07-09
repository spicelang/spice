; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [15 x i8] c"Cell [1,3]: %d\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %a = alloca [10 x [10 x i32]], align 4
  %i = alloca i32, align 4
  %0 = alloca i32, align 4
  %j = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  %5 = alloca i32, align 4
  %6 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store [10 x [10 x i32]] zeroinitializer, [10 x [10 x i32]]* %a, align 4
  store i32 0, i32* %0, align 4
  br label %for.cond3

for:                                              ; preds = %for.cond3
  store i32 0, i32* %1, align 4
  br label %for.cond

for1:                                             ; preds = %for.cond
  %7 = load i32, i32* %0, align 4
  %8 = load i32, i32* %1, align 4
  %9 = mul i32 %7, %8
  store i32 %9, i32* %2, align 4
  %10 = load i32, i32* %2, align 4
  %11 = load [10 x [10 x i32]], [10 x [10 x i32]]* %a, align 4
  %12 = load i32, i32* %0, align 4
  %13 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %a, i32 0, i32 %12
  %14 = load [10 x i32], [10 x i32]* %13, align 4
  %15 = load i32, i32* %1, align 4
  %16 = getelementptr inbounds [10 x i32], [10 x i32]* %13, i32 0, i32 %15
  store i32 %10, i32* %16, align 4
  br label %for.inc

for.inc:                                          ; preds = %for1
  %17 = load i32, i32* %1, align 4
  %18 = add i32 %17, 1
  store i32 %18, i32* %1, align 4
  store i32 %17, i32* %3, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %for
  %19 = load i32, i32* %1, align 4
  %20 = icmp slt i32 %19, 10
  store i1 %20, i1* %4, align 1
  %21 = load i1, i1* %4, align 1
  br i1 %21, label %for1, label %for.end

for.end:                                          ; preds = %for.cond
  br label %for.inc2

for.inc2:                                         ; preds = %for.end
  %22 = load i32, i32* %0, align 4
  %23 = add i32 %22, 1
  store i32 %23, i32* %0, align 4
  store i32 %22, i32* %5, align 4
  br label %for.cond3

for.cond3:                                        ; preds = %for.inc2, %entry
  %24 = load i32, i32* %0, align 4
  %25 = icmp slt i32 %24, 10
  store i1 %25, i1* %6, align 1
  %26 = load i1, i1* %6, align 1
  br i1 %26, label %for, label %for.end4

for.end4:                                         ; preds = %for.cond3
  %27 = load [10 x [10 x i32]], [10 x [10 x i32]]* %a, align 4
  %28 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %a, i32 0, i32 1
  %29 = load [10 x i32], [10 x i32]* %28, align 4
  %30 = getelementptr inbounds [10 x i32], [10 x i32]* %28, i32 0, i32 3
  %31 = load i32, i32* %30, align 4
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @0, i32 0, i32 0), i32 %31)
  %33 = load i32, i32* %result, align 4
  ret i32 %33
}

declare i32 @printf(i8*, ...)
