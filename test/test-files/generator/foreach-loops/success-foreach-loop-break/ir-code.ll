; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@anonymous.0 = global [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %s1 = alloca i16, align 2
  store i16 5, ptr %s1, align 2
  %0 = load i16, ptr %s1, align 2
  %1 = alloca i16, align 2
  store i16 -2, ptr %1, align 2
  %2 = load i16, ptr %1, align 2
  %shortArray.3 = alloca i16, align 2
  store i16 63, ptr %shortArray.3, align 2
  %3 = load i16, ptr %shortArray.3, align 2
  %4 = sub i16 0, %3
  %5 = alloca i16, align 2
  store i16 %4, ptr %5, align 2
  %6 = load i16, ptr %5, align 2
  %7 = alloca [5 x i16], align 2
  %8 = getelementptr inbounds [5 x i16], ptr %7, i32 0, i32 0
  store i16 1, ptr %8, align 2
  %9 = getelementptr inbounds [5 x i16], ptr %7, i32 0, i32 1
  store i16 %0, ptr %9, align 2
  %10 = getelementptr inbounds [5 x i16], ptr %7, i32 0, i32 2
  store i16 %2, ptr %10, align 2
  %11 = getelementptr inbounds [5 x i16], ptr %7, i32 0, i32 3
  store i16 %6, ptr %11, align 2
  %12 = getelementptr inbounds [5 x i16], ptr %7, i32 0, i32 4
  store i16 0, ptr %12, align 2
  %idx = alloca i32, align 4
  store i32 0, ptr %idx, align 4
  %s = alloca i16, align 2
  store i16 0, ptr %s, align 2
  %13 = load [5 x i16], ptr %7, align 2
  %14 = load i32, ptr %idx, align 4
  %15 = getelementptr inbounds [5 x i16], ptr %7, i32 0, i32 %14
  %16 = load i16, ptr %15, align 2
  store i16 %16, ptr %s, align 2
  br label %foreach.loop.l4

foreach.loop.l4:                                  ; preds = %foreach.cond.l4, %entry.l1
  %17 = load i16, ptr %s, align 2
  %18 = sext i16 %17 to i32
  %19 = call i32 (ptr, ...) @printf(ptr @0, i32 %18)
  %20 = load i16, ptr %s, align 2
  %21 = and i16 %20, 1
  %22 = alloca i16, align 2
  store i16 %21, ptr %22, align 2
  %23 = load i16, ptr %22, align 2
  %24 = sext i16 %23 to i32
  %25 = icmp eq i32 %24, 1
  %26 = alloca i1, align 1
  store i1 %25, ptr %26, align 1
  %27 = load i1, ptr %26, align 1
  br i1 %27, label %if.then.l6, label %if.end.l6

if.then.l6:                                       ; preds = %foreach.loop.l4
  %idx1 = alloca i32, align 4
  store i32 0, ptr %idx1, align 4
  %l = alloca i64, align 8
  store i64 0, ptr %l, align 8
  %28 = load [2 x i64], ptr @anonymous.0, align 8
  %29 = load i32, ptr %idx1, align 4
  %30 = getelementptr inbounds [2 x i64], ptr @anonymous.0, i32 0, i32 %29
  %31 = load i64, ptr %30, align 8
  store i64 %31, ptr %l, align 8
  br label %foreach.loop.l7

foreach.loop.l7:                                  ; preds = %foreach.cond.l7, %if.then.l6
  %32 = load i64, ptr %l, align 8
  %33 = call i32 (ptr, ...) @printf(ptr @1, i64 %32)
  br label %foreach.end.l4

foreach.inc.l7:                                   ; No predecessors!
  %idx2 = load i32, ptr %idx1, align 4
  %idx.inc = add i32 %idx2, 1
  store i32 %idx.inc, ptr %idx1, align 4
  %34 = getelementptr inbounds [2 x i64], ptr @anonymous.0, i32 0, i32 %idx.inc
  %35 = load i64, ptr %34, align 8
  store i64 %35, ptr %l, align 8
  br label %foreach.cond.l7

foreach.cond.l7:                                  ; preds = %foreach.inc.l7
  %36 = load i32, ptr %idx1, align 4
  %37 = icmp ult i32 %36, 2
  br i1 %37, label %foreach.loop.l7, label %foreach.end.l7

foreach.end.l7:                                   ; preds = %foreach.cond.l7
  br label %if.end.l6

if.end.l6:                                        ; preds = %foreach.end.l7, %foreach.loop.l4
  br label %foreach.inc.l4

foreach.inc.l4:                                   ; preds = %if.end.l6
  %idx3 = load i32, ptr %idx, align 4
  %idx.inc4 = add i32 %idx3, 1
  store i32 %idx.inc4, ptr %idx, align 4
  %38 = getelementptr inbounds [5 x i16], ptr %7, i32 0, i32 %idx.inc4
  %39 = load i16, ptr %38, align 2
  store i16 %39, ptr %s, align 2
  br label %foreach.cond.l4

foreach.cond.l4:                                  ; preds = %foreach.inc.l4
  %40 = load i32, ptr %idx, align 4
  %41 = icmp ult i32 %40, 5
  br i1 %41, label %foreach.loop.l4, label %foreach.end.l4

foreach.end.l4:                                   ; preds = %foreach.cond.l4, %foreach.loop.l7
  %42 = call i32 (ptr, ...) @printf(ptr @2)
  %43 = load i32, ptr %result, align 4
  ret i32 %43
}

declare i32 @printf(ptr, ...)
