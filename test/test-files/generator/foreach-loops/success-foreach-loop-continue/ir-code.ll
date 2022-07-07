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
  %shortArray = alloca [5 x i16], align 2
  %0 = alloca i16, align 2
  %1 = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  %4 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %5 = alloca i16, align 2
  %6 = alloca i1, align 1
  %idx1 = alloca i32, align 4
  %l = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  store i32 0, i32* %result, align 4
  store i16 5, i16* %s1, align 2
  store i16 1, i16* %0, align 2
  store i16 25, i16* %1, align 2
  store i16 27, i16* %2, align 2
  %9 = load i16, i16* %2, align 2
  %10 = sub i16 0, %9
  store i16 %10, i16* %2, align 2
  store i16 63, i16* %3, align 2
  %11 = load i16, i16* %3, align 2
  %12 = sub i16 0, %11
  store i16 %12, i16* %3, align 2
  %13 = load i16, i16* %0, align 2
  %14 = getelementptr inbounds [5 x i16], [5 x i16]* %4, i32 0, i32 0
  store i16 %13, i16* %14, align 2
  %15 = load i16, i16* %s1, align 2
  %16 = getelementptr inbounds [5 x i16], [5 x i16]* %4, i32 0, i32 1
  store i16 %15, i16* %16, align 2
  %17 = load i16, i16* %1, align 2
  %18 = getelementptr inbounds [5 x i16], [5 x i16]* %4, i32 0, i32 2
  store i16 %17, i16* %18, align 2
  %19 = load i16, i16* %2, align 2
  %20 = getelementptr inbounds [5 x i16], [5 x i16]* %4, i32 0, i32 3
  store i16 %19, i16* %20, align 2
  %21 = load i16, i16* %3, align 2
  %22 = getelementptr inbounds [5 x i16], [5 x i16]* %4, i32 0, i32 4
  store i16 %21, i16* %22, align 2
  %23 = load [5 x i16], [5 x i16]* %4, align 2
  store [5 x i16] %23, [5 x i16]* %shortArray, align 2
  store i32 0, i32* %idx, align 4
  store i16 0, i16* %s, align 2
  %24 = load [5 x i16], [5 x i16]* %shortArray, align 2
  %25 = load i32, i32* %idx, align 4
  %26 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i32 0, i32 %25
  %27 = load i16, i16* %26, align 2
  store i16 %27, i16* %s, align 2
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.cond7, %entry
  %28 = load i16, i16* %s, align 2
  %29 = sext i16 %28 to i32
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %29)
  %31 = load i16, i16* %s, align 2
  %32 = and i16 %31, 1
  store i16 %32, i16* %5, align 2
  %33 = load i16, i16* %5, align 2
  %34 = sext i16 %33 to i32
  %35 = icmp eq i32 %34, 1
  store i1 %35, i1* %6, align 1
  %36 = load i1, i1* %6, align 1
  br i1 %36, label %if.then, label %if.end

if.then:                                          ; preds = %foreach.loop
  store i32 0, i32* %idx1, align 4
  store i64 0, i64* %l, align 8
  store i64 1, i64* %7, align 8
  store i64 2, i64* %8, align 8
  %37 = load [2 x i64], [2 x i64]* @l, align 8
  %38 = load i32, i32* %idx1, align 4
  %39 = getelementptr inbounds [2 x i64], [2 x i64]* @l, i32 0, i32 %38
  %40 = load i64, i64* %39, align 8
  store i64 %40, i64* %l, align 8
  br label %foreach.loop2

foreach.loop2:                                    ; preds = %foreach.cond, %if.then
  %41 = load i64, i64* %l, align 8
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @1, i32 0, i32 0), i64 %41)
  br label %foreach.inc4

foreach.inc:                                      ; No predecessors!
  %idx3 = load i32, i32* %idx1, align 4
  %idx.inc = add i32 %idx3, 1
  store i32 %idx.inc, i32* %idx1, align 4
  %43 = getelementptr inbounds [2 x i64], [2 x i64]* @l, i32 0, i32 %idx.inc
  %44 = load i64, i64* %43, align 8
  store i64 %44, i64* %l, align 8
  br label %foreach.cond

foreach.cond:                                     ; preds = %foreach.inc
  %45 = load i32, i32* %idx1, align 4
  %46 = icmp ule i32 %45, 1
  br i1 %46, label %foreach.loop2, label %foreach.end

foreach.end:                                      ; preds = %foreach.cond
  br label %if.end

if.end:                                           ; preds = %foreach.end, %foreach.loop
  br label %foreach.inc4

foreach.inc4:                                     ; preds = %if.end, %foreach.loop2
  %idx5 = load i32, i32* %idx, align 4
  %idx.inc6 = add i32 %idx5, 1
  store i32 %idx.inc6, i32* %idx, align 4
  %47 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i32 0, i32 %idx.inc6
  %48 = load i16, i16* %47, align 2
  store i16 %48, i16* %s, align 2
  br label %foreach.cond7

foreach.cond7:                                    ; preds = %foreach.inc4
  %49 = load i32, i32* %idx, align 4
  %50 = icmp ule i32 %49, 4
  br i1 %50, label %foreach.loop, label %foreach.end8

foreach.end8:                                     ; preds = %foreach.cond7
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %52 = load i32, i32* %result, align 4
  ret i32 %52
}

declare i32 @printf(i8*, ...)
