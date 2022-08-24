; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %counter = alloca i32, align 4
  store i32 0, ptr %counter, align 4
  br label %for.cond.l2

for.l2:                                           ; preds = %for.cond.l2
  %0 = load i32, ptr %counter, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 %0)
  %2 = load i32, ptr %counter, align 4
  %3 = icmp sge i32 %2, 9001
  %4 = alloca i1, align 1
  store i1 %3, ptr %4, align 1
  %5 = load i1, ptr %4, align 1
  br i1 %5, label %if.then.l4, label %if.end.l4

if.then.l4:                                       ; preds = %for.l2
  %subCounter = alloca i32, align 4
  store i32 100, ptr %subCounter, align 4
  br label %for.cond.l5

for.l5:                                           ; preds = %for.cond.l5
  %6 = load i32, ptr %subCounter, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @1, i32 %6)
  %8 = load i32, ptr %subCounter, align 4
  %9 = icmp eq i32 %8, 11
  %10 = alloca i1, align 1
  store i1 %9, ptr %10, align 1
  %11 = load i1, ptr %10, align 1
  br i1 %11, label %if.then.l7, label %if.end.l7

if.then.l7:                                       ; preds = %for.l5
  br label %for.end.l2

if.end.l7:                                        ; preds = %for.l5
  br label %for.inc.l5

for.inc.l5:                                       ; preds = %if.end.l7
  %12 = load i32, ptr %subCounter, align 4
  %13 = sub i32 %12, 1
  store i32 %13, ptr %subCounter, align 4
  br label %for.cond.l5

for.cond.l5:                                      ; preds = %for.inc.l5, %if.then.l4
  %14 = load i32, ptr %subCounter, align 4
  %15 = icmp sge i32 %14, 10
  %16 = alloca i1, align 1
  store i1 %15, ptr %16, align 1
  %17 = load i1, ptr %16, align 1
  br i1 %17, label %for.l5, label %for.end.l5

for.end.l5:                                       ; preds = %for.cond.l5
  br label %if.end.l4

if.end.l4:                                        ; preds = %for.end.l5, %for.l2
  br label %for.inc.l2

for.inc.l2:                                       ; preds = %if.end.l4
  %18 = load i32, ptr %counter, align 4
  %19 = add i32 %18, 2
  store i32 %19, ptr %counter, align 4
  br label %for.cond.l2

for.cond.l2:                                      ; preds = %for.inc.l2, %entry.l1
  %20 = load i32, ptr %counter, align 4
  %21 = icmp slt i32 %20, 10000
  %22 = alloca i1, align 1
  store i1 %21, ptr %22, align 1
  %23 = load i1, ptr %22, align 1
  br i1 %23, label %for.l2, label %for.end.l2

for.end.l2:                                       ; preds = %for.cond.l2, %if.then.l7
  %24 = call i32 (ptr, ...) @printf(ptr @2)
  %25 = load i32, ptr %result, align 4
  ret i32 %25
}

declare i32 @printf(ptr, ...)
