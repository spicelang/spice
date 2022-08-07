; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [15 x i8] c"Cell [1,3]: %d\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %a = alloca [10 x [10 x i32]], align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store [10 x [10 x i32]] zeroinitializer, [10 x [10 x i32]]* %a, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond.l3

for.l3:                                           ; preds = %for.cond.l3
  store i32 0, i32* %j, align 4
  br label %for.cond.l4

for.l4:                                           ; preds = %for.cond.l4
  %5 = load i32, i32* %i, align 4
  %6 = load i32, i32* %j, align 4
  %7 = mul i32 %5, %6
  store i32 %7, i32* %0, align 4
  %8 = load i32, i32* %0, align 4
  %9 = load [10 x [10 x i32]], [10 x [10 x i32]]* %a, align 4
  %10 = load i32, i32* %i, align 4
  %11 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %a, i32 0, i32 %10
  %12 = load [10 x i32], [10 x i32]* %11, align 4
  %13 = load i32, i32* %j, align 4
  %14 = getelementptr inbounds [10 x i32], [10 x i32]* %11, i32 0, i32 %13
  store i32 %8, i32* %14, align 4
  br label %for.inc.l4

for.inc.l4:                                       ; preds = %for.l4
  %15 = load i32, i32* %j, align 4
  %16 = add i32 %15, 1
  store i32 %16, i32* %j, align 4
  store i32 %15, i32* %1, align 4
  br label %for.cond.l4

for.cond.l4:                                      ; preds = %for.inc.l4, %for.l3
  %17 = load i32, i32* %j, align 4
  %18 = icmp slt i32 %17, 10
  store i1 %18, i1* %2, align 1
  %19 = load i1, i1* %2, align 1
  br i1 %19, label %for.l4, label %for.end.l4

for.end.l4:                                       ; preds = %for.cond.l4
  br label %for.inc.l3

for.inc.l3:                                       ; preds = %for.end.l4
  %20 = load i32, i32* %i, align 4
  %21 = add i32 %20, 1
  store i32 %21, i32* %i, align 4
  store i32 %20, i32* %3, align 4
  br label %for.cond.l3

for.cond.l3:                                      ; preds = %for.inc.l3, %entry.l1
  %22 = load i32, i32* %i, align 4
  %23 = icmp slt i32 %22, 10
  store i1 %23, i1* %4, align 1
  %24 = load i1, i1* %4, align 1
  br i1 %24, label %for.l3, label %for.end.l3

for.end.l3:                                       ; preds = %for.cond.l3
  %25 = load [10 x [10 x i32]], [10 x [10 x i32]]* %a, align 4
  %26 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %a, i32 0, i32 1
  %27 = load [10 x i32], [10 x i32]* %26, align 4
  %28 = getelementptr inbounds [10 x i32], [10 x i32]* %26, i32 0, i32 3
  %29 = load i32, i32* %28, align 4
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @0, i32 0, i32 0), i32 %29)
  %31 = load i32, i32* %result, align 4
  ret i32 %31
}

declare i32 @printf(i8*, ...)
