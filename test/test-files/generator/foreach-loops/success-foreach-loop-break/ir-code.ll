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
  %shortArray = alloca [5 x i16], align 2
  %1 = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  %5 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %6 = alloca i16, align 2
  %7 = alloca i1, align 1
  %idx1 = alloca i32, align 4
  %l = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca i64, align 8
  %l2 = alloca [2 x i64], align 8
  store i32 0, i32* %result, align 4
  store i16 5, i16* %0, align 2
  store i16 1, i16* %1, align 2
  store i16 -2, i16* %2, align 2
  store i16 63, i16* %3, align 2
  %10 = load i16, i16* %3, align 2
  %11 = sub i16 0, %10
  store i16 %11, i16* %4, align 2
  %12 = load i16, i16* %1, align 2
  %13 = getelementptr inbounds [5 x i16], [5 x i16]* %5, i32 0, i32 0
  store i16 %12, i16* %13, align 2
  %14 = load i16, i16* %0, align 2
  %15 = getelementptr inbounds [5 x i16], [5 x i16]* %5, i32 0, i32 1
  store i16 %14, i16* %15, align 2
  %16 = load i16, i16* %2, align 2
  %17 = getelementptr inbounds [5 x i16], [5 x i16]* %5, i32 0, i32 2
  store i16 %16, i16* %17, align 2
  %18 = load i16, i16* %4, align 2
  %19 = getelementptr inbounds [5 x i16], [5 x i16]* %5, i32 0, i32 3
  store i16 %18, i16* %19, align 2
  %20 = getelementptr inbounds [5 x i16], [5 x i16]* %5, i32 0, i32 4
  store i16 0, i16* %20, align 2
  store i32 0, i32* %idx, align 4
  store i16 0, i16* %s, align 2
  %21 = load [5 x i16], [5 x i16]* %5, align 2
  %22 = load i32, i32* %idx, align 4
  %23 = getelementptr inbounds [5 x i16], [5 x i16]* %5, i32 0, i32 %22
  %24 = load i16, i16* %23, align 2
  store i16 %24, i16* %s, align 2
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.cond8, %entry
  %25 = load i16, i16* %s, align 2
  %26 = sext i16 %25 to i32
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %26)
  %28 = load i16, i16* %s, align 2
  %29 = and i16 %28, 1
  store i16 %29, i16* %6, align 2
  %30 = load i16, i16* %6, align 2
  %31 = sext i16 %30 to i32
  %32 = icmp eq i32 %31, 1
  store i1 %32, i1* %7, align 1
  %33 = load i1, i1* %7, align 1
  br i1 %33, label %if.then, label %if.end

if.then:                                          ; preds = %foreach.loop
  store i32 0, i32* %idx1, align 4
  store i64 0, i64* %l, align 8
  store i64 1, i64* %8, align 8
  store i64 2, i64* %9, align 8
  store [2 x i64] [i64 1, i64 2], [2 x i64]* %l2, align 8
  %34 = load [2 x i64], [2 x i64]* %l2, align 8
  %35 = load i32, i32* %idx1, align 4
  %36 = getelementptr inbounds [2 x i64], [2 x i64]* %l2, i32 0, i32 %35
  %37 = load i64, i64* %36, align 8
  store i64 %37, i64* %l, align 8
  br label %foreach.loop3

foreach.loop3:                                    ; preds = %foreach.cond, %if.then
  %38 = load i64, i64* %l, align 8
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @1, i32 0, i32 0), i64 %38)
  br label %foreach.end9

foreach.inc:                                      ; No predecessors!
  %idx4 = load i32, i32* %idx1, align 4
  %idx.inc = add i32 %idx4, 1
  store i32 %idx.inc, i32* %idx1, align 4
  %40 = getelementptr inbounds [2 x i64], [2 x i64]* %l2, i32 0, i32 %idx.inc
  %41 = load i64, i64* %40, align 8
  store i64 %41, i64* %l, align 8
  br label %foreach.cond

foreach.cond:                                     ; preds = %foreach.inc
  %42 = load i32, i32* %idx1, align 4
  %43 = icmp ule i32 %42, 1
  br i1 %43, label %foreach.loop3, label %foreach.end

foreach.end:                                      ; preds = %foreach.cond
  br label %if.end

if.end:                                           ; preds = %foreach.end, %foreach.loop
  br label %foreach.inc5

foreach.inc5:                                     ; preds = %if.end
  %idx6 = load i32, i32* %idx, align 4
  %idx.inc7 = add i32 %idx6, 1
  store i32 %idx.inc7, i32* %idx, align 4
  %44 = getelementptr inbounds [5 x i16], [5 x i16]* %5, i32 0, i32 %idx.inc7
  %45 = load i16, i16* %44, align 2
  store i16 %45, i16* %s, align 2
  br label %foreach.cond8

foreach.cond8:                                    ; preds = %foreach.inc5
  %46 = load i32, i32* %idx, align 4
  %47 = icmp ule i32 %46, 4
  br i1 %47, label %foreach.loop, label %foreach.end9

foreach.end9:                                     ; preds = %foreach.cond8, %foreach.loop3
  %48 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %49 = load i32, i32* %result, align 4
  ret i32 %49
}

declare i32 @printf(i8*, ...)
