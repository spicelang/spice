; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [15 x i8] c"Cell [1,3]: %d\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %a = alloca [10 x [10 x i32]], align 4
  store [10 x [10 x i32]] zeroinitializer, ptr %a, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l3

for.l3:                                           ; preds = %for.cond.l3
  %j = alloca i32, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond.l4

for.l4:                                           ; preds = %for.cond.l4
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %j, align 4
  %2 = mul i32 %0, %1
  %3 = alloca i32, align 4
  store i32 %2, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  %5 = load i32, ptr %i, align 4
  %6 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i32 0, i32 %5
  %7 = load i32, ptr %j, align 4
  %8 = getelementptr inbounds [10 x i32], ptr %6, i32 0, i32 %7
  store i32 %4, ptr %8, align 4
  br label %for.inc.l4

for.inc.l4:                                       ; preds = %for.l4
  %9 = load i32, ptr %j, align 4
  %10 = add i32 %9, 1
  store i32 %10, ptr %j, align 4
  %11 = alloca i32, align 4
  store i32 %9, ptr %11, align 4
  br label %for.cond.l4

for.cond.l4:                                      ; preds = %for.inc.l4, %for.l3
  %12 = load i32, ptr %j, align 4
  %13 = icmp slt i32 %12, 10
  %14 = alloca i1, align 1
  store i1 %13, ptr %14, align 1
  %15 = load i1, ptr %14, align 1
  br i1 %15, label %for.l4, label %for.end.l4

for.end.l4:                                       ; preds = %for.cond.l4
  br label %for.inc.l3

for.inc.l3:                                       ; preds = %for.end.l4
  %16 = load i32, ptr %i, align 4
  %17 = add i32 %16, 1
  store i32 %17, ptr %i, align 4
  %18 = alloca i32, align 4
  store i32 %16, ptr %18, align 4
  br label %for.cond.l3

for.cond.l3:                                      ; preds = %for.inc.l3, %entry.l1
  %19 = load i32, ptr %i, align 4
  %20 = icmp slt i32 %19, 10
  %21 = alloca i1, align 1
  store i1 %20, ptr %21, align 1
  %22 = load i1, ptr %21, align 1
  br i1 %22, label %for.l3, label %for.end.l3

for.end.l3:                                       ; preds = %for.cond.l3
  %23 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i32 0, i32 1
  %24 = getelementptr inbounds [10 x i32], ptr %23, i32 0, i32 3
  %25 = load i32, ptr %24, align 4
  %26 = call i32 (ptr, ...) @printf(ptr @0, i32 %25)
  %27 = load i32, ptr %result, align 4
  ret i32 %27
}

declare i32 @printf(ptr, ...)
