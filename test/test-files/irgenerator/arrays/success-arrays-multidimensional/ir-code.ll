; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [15 x i8] c"Cell [1,3]: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %a = alloca [10 x [10 x i32]], align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store [10 x [10 x i32]] zeroinitializer, ptr %a, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L3

for.head.L3:                                      ; preds = %for.tail.L3, %0
  %3 = load i32, ptr %i, align 4
  %4 = icmp slt i32 %3, 10
  br i1 %4, label %for.body.L3, label %for.exit.L3

for.body.L3:                                      ; preds = %for.head.L3
  store i32 0, ptr %j, align 4
  br label %for.head.L4

for.head.L4:                                      ; preds = %for.tail.L4, %for.body.L3
  %5 = load i32, ptr %j, align 4
  %6 = icmp slt i32 %5, 10
  br i1 %6, label %for.body.L4, label %for.exit.L4

for.body.L4:                                      ; preds = %for.head.L4
  %7 = load i32, ptr %i, align 4
  %8 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i32 0, i32 %7
  %9 = load i32, ptr %j, align 4
  %10 = getelementptr inbounds [10 x i32], ptr %8, i32 0, i32 %9
  %11 = load i32, ptr %j, align 4
  %12 = load i32, ptr %i, align 4
  %13 = mul i32 %12, %11
  store i32 %13, ptr %10, align 4
  br label %for.tail.L4

for.tail.L4:                                      ; preds = %for.body.L4
  %14 = load i32, ptr %j, align 4
  %15 = add i32 %14, 1
  store i32 %15, ptr %j, align 4
  store i32 %14, ptr %1, align 4
  br label %for.head.L4

for.exit.L4:                                      ; preds = %for.head.L4
  br label %for.tail.L3

for.tail.L3:                                      ; preds = %for.exit.L4
  %16 = load i32, ptr %i, align 4
  %17 = add i32 %16, 1
  store i32 %17, ptr %i, align 4
  store i32 %16, ptr %2, align 4
  br label %for.head.L3

for.exit.L3:                                      ; preds = %for.head.L3
  %18 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i32 0, i32 1
  %19 = getelementptr inbounds [10 x i32], ptr %18, i32 0, i32 3
  %20 = load i32, ptr %19, align 4
  %21 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %20)
  %22 = load i32, ptr %result, align 4
  ret i32 %22
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
