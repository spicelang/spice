; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@l = constant [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %s1 = alloca i16, align 2
  %0 = alloca i16, align 2
  %shortArray = alloca i16, align 2
  %1 = alloca i16, align 2
  %2 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %3 = alloca i16, align 2
  %4 = alloca i1, align 1
  %idx1 = alloca i32, align 4
  %l = alloca i64, align 8
  %l2 = alloca [2 x i64], align 8
  store i32 0, i32* %result, align 4
  store i16 5, i16* %s1, align 2
  %5 = load i16, i16* %s1, align 2
  store i16 -2, i16* %0, align 2
  %6 = load i16, i16* %0, align 2
  store i16 63, i16* %shortArray, align 2
  %7 = load i16, i16* %shortArray, align 2
  %8 = sub i16 0, %7
  store i16 %8, i16* %1, align 2
  %9 = load i16, i16* %1, align 2
  %10 = getelementptr inbounds [5 x i16], [5 x i16]* %2, i32 0, i32 0
  store i16 1, i16* %10, align 2
  %11 = getelementptr inbounds [5 x i16], [5 x i16]* %2, i32 0, i32 1
  store i16 %5, i16* %11, align 2
  %12 = getelementptr inbounds [5 x i16], [5 x i16]* %2, i32 0, i32 2
  store i16 %6, i16* %12, align 2
  %13 = getelementptr inbounds [5 x i16], [5 x i16]* %2, i32 0, i32 3
  store i16 %9, i16* %13, align 2
  %14 = getelementptr inbounds [5 x i16], [5 x i16]* %2, i32 0, i32 4
  store i16 0, i16* %14, align 2
  store i32 0, i32* %idx, align 4
  store i16 0, i16* %s, align 2
  %15 = load [5 x i16], [5 x i16]* %2, align 2
  %16 = load i32, i32* %idx, align 4
  %17 = getelementptr inbounds [5 x i16], [5 x i16]* %2, i32 0, i32 %16
  %18 = load i16, i16* %17, align 2
  store i16 %18, i16* %s, align 2
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.cond8, %entry
  %19 = load i16, i16* %s, align 2
  %20 = sext i16 %19 to i32
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %20)
  %22 = load i16, i16* %s, align 2
  %23 = and i16 %22, 1
  store i16 %23, i16* %3, align 2
  %24 = load i16, i16* %3, align 2
  %25 = sext i16 %24 to i32
  %26 = icmp eq i32 %25, 1
  store i1 %26, i1* %4, align 1
  %27 = load i1, i1* %4, align 1
  br i1 %27, label %if.then, label %if.end

if.then:                                          ; preds = %foreach.loop
  store i32 0, i32* %idx1, align 4
  store i64 0, i64* %l, align 8
  store [2 x i64] [i64 1, i64 2], [2 x i64]* %l2, align 8
  %28 = load [2 x i64], [2 x i64]* %l2, align 8
  %29 = load i32, i32* %idx1, align 4
  %30 = getelementptr inbounds [2 x i64], [2 x i64]* %l2, i32 0, i32 %29
  %31 = load i64, i64* %30, align 8
  store i64 %31, i64* %l, align 8
  br label %foreach.loop3

foreach.loop3:                                    ; preds = %foreach.cond, %if.then
  %32 = load i64, i64* %l, align 8
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @1, i32 0, i32 0), i64 %32)
  br label %foreach.end9

foreach.inc:                                      ; No predecessors!
  %idx4 = load i32, i32* %idx1, align 4
  %idx.inc = add i32 %idx4, 1
  store i32 %idx.inc, i32* %idx1, align 4
  %34 = getelementptr inbounds [2 x i64], [2 x i64]* %l2, i32 0, i32 %idx.inc
  %35 = load i64, i64* %34, align 8
  store i64 %35, i64* %l, align 8
  br label %foreach.cond

foreach.cond:                                     ; preds = %foreach.inc
  %36 = load i32, i32* %idx1, align 4
  %37 = icmp ule i32 %36, 1
  br i1 %37, label %foreach.loop3, label %foreach.end

foreach.end:                                      ; preds = %foreach.cond
  br label %if.end

if.end:                                           ; preds = %foreach.end, %foreach.loop
  br label %foreach.inc5

foreach.inc5:                                     ; preds = %if.end
  %idx6 = load i32, i32* %idx, align 4
  %idx.inc7 = add i32 %idx6, 1
  store i32 %idx.inc7, i32* %idx, align 4
  %38 = getelementptr inbounds [5 x i16], [5 x i16]* %2, i32 0, i32 %idx.inc7
  %39 = load i16, i16* %38, align 2
  store i16 %39, i16* %s, align 2
  br label %foreach.cond8

foreach.cond8:                                    ; preds = %foreach.inc5
  %40 = load i32, i32* %idx, align 4
  %41 = icmp ule i32 %40, 4
  br i1 %41, label %foreach.loop, label %foreach.end9

foreach.end9:                                     ; preds = %foreach.cond8, %foreach.loop3
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %43 = load i32, i32* %result, align 4
  ret i32 %43
}

declare i32 @printf(i8*, ...)
