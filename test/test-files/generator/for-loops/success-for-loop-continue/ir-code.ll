; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %counter = alloca i32, align 4
  %0 = alloca i1, align 1
  %subCounter = alloca i32, align 4
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %3 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store i32 0, ptr %counter, align 4
  br label %for.cond.l2

for.l2:                                           ; preds = %for.cond.l2
  %4 = load i32, ptr %counter, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0, i32 %4)
  %6 = load i32, ptr %counter, align 4
  %7 = icmp sge i32 %6, 5
  store i1 %7, ptr %0, align 1
  %8 = load i1, ptr %0, align 1
  br i1 %8, label %if.then.l4, label %if.end.l4

if.then.l4:                                       ; preds = %for.l2
  store i32 100, ptr %subCounter, align 4
  br label %for.cond.l5

for.l5:                                           ; preds = %for.cond.l5
  %9 = load i32, ptr %subCounter, align 4
  %10 = call i32 (ptr, ...) @printf(ptr @1, i32 %9)
  %11 = load i32, ptr %subCounter, align 4
  %12 = icmp eq i32 %11, 11
  store i1 %12, ptr %1, align 1
  %13 = load i1, ptr %1, align 1
  br i1 %13, label %if.then.l7, label %if.end.l7

if.then.l7:                                       ; preds = %for.l5
  br label %for.inc.l2

if.end.l7:                                        ; preds = %for.l5
  br label %for.inc.l5

for.inc.l5:                                       ; preds = %if.end.l7
  %14 = load i32, ptr %subCounter, align 4
  %15 = sub i32 %14, 1
  store i32 %15, ptr %subCounter, align 4
  br label %for.cond.l5

for.cond.l5:                                      ; preds = %for.inc.l5, %if.then.l4
  %16 = load i32, ptr %subCounter, align 4
  %17 = icmp sge i32 %16, 10
  store i1 %17, ptr %2, align 1
  %18 = load i1, ptr %2, align 1
  br i1 %18, label %for.l5, label %for.end.l5

for.end.l5:                                       ; preds = %for.cond.l5
  br label %if.end.l4

if.end.l4:                                        ; preds = %for.end.l5, %for.l2
  br label %for.inc.l2

for.inc.l2:                                       ; preds = %if.end.l4, %if.then.l7
  %19 = load i32, ptr %counter, align 4
  %20 = add i32 %19, 2
  store i32 %20, ptr %counter, align 4
  br label %for.cond.l2

for.cond.l2:                                      ; preds = %for.inc.l2, %entry.l1
  %21 = load i32, ptr %counter, align 4
  %22 = icmp slt i32 %21, 10
  store i1 %22, ptr %3, align 1
  %23 = load i1, ptr %3, align 1
  br i1 %23, label %for.l2, label %for.end.l2

for.end.l2:                                       ; preds = %for.cond.l2
  %24 = call i32 (ptr, ...) @printf(ptr @2)
  %25 = load i32, ptr %result, align 4
  ret i32 %25
}

declare i32 @printf(ptr, ...)
