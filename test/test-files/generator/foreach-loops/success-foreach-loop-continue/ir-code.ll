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
  %shortArray.3 = alloca i16, align 2
  store i16 27, ptr %shortArray.3, align 2
  %1 = load i16, ptr %shortArray.3, align 2
  %2 = sub i16 0, %1
  %3 = alloca i16, align 2
  store i16 %2, ptr %3, align 2
  %4 = load i16, ptr %3, align 2
  %shortArray.4 = alloca i16, align 2
  store i16 63, ptr %shortArray.4, align 2
  %5 = load i16, ptr %shortArray.4, align 2
  %6 = sub i16 0, %5
  %7 = alloca i16, align 2
  store i16 %6, ptr %7, align 2
  %8 = load i16, ptr %7, align 2
  %9 = alloca [5 x i16], align 2
  %10 = getelementptr inbounds [5 x i16], ptr %9, i32 0, i32 0
  store i16 1, ptr %10, align 2
  %11 = getelementptr inbounds [5 x i16], ptr %9, i32 0, i32 1
  store i16 %0, ptr %11, align 2
  %12 = getelementptr inbounds [5 x i16], ptr %9, i32 0, i32 2
  store i16 25, ptr %12, align 2
  %13 = getelementptr inbounds [5 x i16], ptr %9, i32 0, i32 3
  store i16 %4, ptr %13, align 2
  %14 = getelementptr inbounds [5 x i16], ptr %9, i32 0, i32 4
  store i16 %8, ptr %14, align 2
  %idx = alloca i32, align 4
  store i32 0, ptr %idx, align 4
  %s = alloca i16, align 2
  store i16 0, ptr %s, align 2
  %15 = load [5 x i16], ptr %9, align 2
  %16 = load i32, ptr %idx, align 4
  %17 = getelementptr inbounds [5 x i16], ptr %9, i32 0, i32 %16
  %18 = load i16, ptr %17, align 2
  store i16 %18, ptr %s, align 2
  br label %foreach.loop.l4

foreach.loop.l4:                                  ; preds = %foreach.cond.l4, %entry.l1
  %19 = load i16, ptr %s, align 2
  %20 = sext i16 %19 to i32
  %21 = call i32 (ptr, ...) @printf(ptr @0, i32 %20)
  %22 = load i16, ptr %s, align 2
  %23 = and i16 %22, 1
  %24 = alloca i16, align 2
  store i16 %23, ptr %24, align 2
  %25 = load i16, ptr %24, align 2
  %26 = sext i16 %25 to i32
  %27 = icmp eq i32 %26, 1
  %28 = alloca i1, align 1
  store i1 %27, ptr %28, align 1
  %29 = load i1, ptr %28, align 1
  br i1 %29, label %if.then.l6, label %if.end.l6

if.then.l6:                                       ; preds = %foreach.loop.l4
  %idx1 = alloca i32, align 4
  store i32 0, ptr %idx1, align 4
  %l = alloca i64, align 8
  store i64 0, ptr %l, align 8
  %30 = load [2 x i64], ptr @anonymous.0, align 8
  %31 = load i32, ptr %idx1, align 4
  %32 = getelementptr inbounds [2 x i64], ptr @anonymous.0, i32 0, i32 %31
  %33 = load i64, ptr %32, align 8
  store i64 %33, ptr %l, align 8
  br label %foreach.loop.l7

foreach.loop.l7:                                  ; preds = %foreach.cond.l7, %if.then.l6
  %34 = load i64, ptr %l, align 8
  %35 = call i32 (ptr, ...) @printf(ptr @1, i64 %34)
  br label %foreach.inc.l4

foreach.inc.l7:                                   ; No predecessors!
  %idx2 = load i32, ptr %idx1, align 4
  %idx.inc = add i32 %idx2, 1
  store i32 %idx.inc, ptr %idx1, align 4
  %36 = getelementptr inbounds [2 x i64], ptr @anonymous.0, i32 0, i32 %idx.inc
  %37 = load i64, ptr %36, align 8
  store i64 %37, ptr %l, align 8
  br label %foreach.cond.l7

foreach.cond.l7:                                  ; preds = %foreach.inc.l7
  %38 = load i32, ptr %idx1, align 4
  %39 = icmp ult i32 %38, 2
  br i1 %39, label %foreach.loop.l7, label %foreach.end.l7

foreach.end.l7:                                   ; preds = %foreach.cond.l7
  br label %if.end.l6

if.end.l6:                                        ; preds = %foreach.end.l7, %foreach.loop.l4
  br label %foreach.inc.l4

foreach.inc.l4:                                   ; preds = %if.end.l6, %foreach.loop.l7
  %idx3 = load i32, ptr %idx, align 4
  %idx.inc4 = add i32 %idx3, 1
  store i32 %idx.inc4, ptr %idx, align 4
  %40 = getelementptr inbounds [5 x i16], ptr %9, i32 0, i32 %idx.inc4
  %41 = load i16, ptr %40, align 2
  store i16 %41, ptr %s, align 2
  br label %foreach.cond.l4

foreach.cond.l4:                                  ; preds = %foreach.inc.l4
  %42 = load i32, ptr %idx, align 4
  %43 = icmp ult i32 %42, 5
  br i1 %43, label %foreach.loop.l4, label %foreach.end.l4

foreach.end.l4:                                   ; preds = %foreach.cond.l4
  %44 = call i32 (ptr, ...) @printf(ptr @2)
  %45 = load i32, ptr %result, align 4
  ret i32 %45
}

declare i32 @printf(ptr, ...)
