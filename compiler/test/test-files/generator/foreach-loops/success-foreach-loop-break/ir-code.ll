; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@l = dso_local constant [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

declare i32 @printf(i8*, ...)

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
  %5 = alloca i16, align 2
  %6 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %7 = alloca i16, align 2
  %8 = alloca i16, align 2
  %9 = alloca i32, align 4
  %10 = alloca i1, align 1
  %idx1 = alloca i32, align 4
  %l = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i64, align 8
  store i32 0, i32* %result, align 4
  store i16 5, i16* %0, align 2
  %13 = load i16, i16* %0, align 2
  store i16 %13, i16* %s1, align 2
  store i16 1, i16* %1, align 2
  store i16 25, i16* %2, align 2
  %14 = load i16, i16* %2, align 2
  store i16 27, i16* %3, align 2
  %15 = load i16, i16* %3, align 2
  %16 = sub i16 %14, %15
  store i16 %16, i16* %4, align 2
  store i16 63, i16* %5, align 2
  %17 = load i16, i16* %5, align 2
  %18 = mul i16 %17, -1
  store i16 %18, i16* %5, align 2
  %19 = load i16, i16* %1, align 2
  %20 = getelementptr inbounds [5 x i16], [5 x i16]* %6, i32 0, i32 0
  store i16 %19, i16* %20, align 2
  %21 = load i16, i16* %s1, align 2
  %22 = getelementptr inbounds [5 x i16], [5 x i16]* %6, i32 0, i32 1
  store i16 %21, i16* %22, align 2
  %23 = load i16, i16* %4, align 2
  %24 = getelementptr inbounds [5 x i16], [5 x i16]* %6, i32 0, i32 2
  store i16 %23, i16* %24, align 2
  %25 = load i16, i16* %5, align 2
  %26 = getelementptr inbounds [5 x i16], [5 x i16]* %6, i32 0, i32 3
  store i16 %25, i16* %26, align 2
  %27 = load [5 x i16], [5 x i16]* %6, align 2
  store [5 x i16] %27, [5 x i16]* %shortArray, align 2
  store i32 0, i32* %idx, align 4
  %28 = load [5 x i16], [5 x i16]* %shortArray, align 2
  %29 = load i32, i32* %idx, align 4
  %30 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i32 0, i32 %29
  %31 = load i16, i16* %30, align 2
  store i16 %31, i16* %s, align 2
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc3, %entry
  %32 = load i16, i16* %s, align 2
  %33 = zext i16 %32 to i32
  %34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %33)
  %35 = load i16, i16* %s, align 2
  store i16 1, i16* %7, align 2
  %36 = load i16, i16* %7, align 2
  %37 = and i16 %35, %36
  store i16 %37, i16* %8, align 2
  store i32 1, i32* %9, align 4
  %38 = load i16, i16* %8, align 2
  %39 = load i32, i32* %9, align 4
  %40 = sext i16 %38 to i32
  %41 = icmp eq i32 %40, %39
  store i1 %41, i1* %10, align 1
  %42 = load i1, i1* %10, align 1
  br i1 %42, label %if.then, label %if.end

if.then:                                          ; preds = %foreach.loop
  store i32 0, i32* %idx1, align 4
  store i64 1, i64* %11, align 8
  store i64 2, i64* %12, align 8
  %43 = load [2 x i64], [2 x i64]* @l, align 8
  %44 = load i32, i32* %idx1, align 4
  %45 = getelementptr inbounds [2 x i64], [2 x i64]* @l, i32 0, i32 %44
  %46 = load i64, i64* %45, align 8
  store i64 %46, i64* %l, align 8
  br label %foreach.loop2

foreach.loop2:                                    ; preds = %foreach.inc, %if.then
  %47 = load i64, i64* %l, align 8
  %48 = trunc i64 %47 to i32
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @1, i32 0, i32 0), i32 %48)
  br label %foreach.end5

foreach.inc:                                      ; No predecessors!
  %50 = load i32, i32* %idx1, align 4
  %foreach_idx_inc = add i32 %50, 1
  store i32 %foreach_idx_inc, i32* %idx1, align 4
  %51 = getelementptr inbounds [2 x i64], [2 x i64]* @l, i32 0, i32 %foreach_idx_inc
  %52 = load i64, i64* %51, align 8
  store i64 %52, i64* %l, align 8
  br label %foreach.loop2

foreach.end:                                      ; No predecessors!
  br label %if.end

if.end:                                           ; preds = %foreach.end, %foreach.loop
  br label %foreach.inc3

foreach.inc3:                                     ; preds = %if.end
  %53 = load i32, i32* %idx, align 4
  %foreach_idx_inc4 = add i32 %53, 1
  store i32 %foreach_idx_inc4, i32* %idx, align 4
  %54 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i32 0, i32 %foreach_idx_inc4
  %55 = load i16, i16* %54, align 2
  store i16 %55, i16* %s, align 2
  br label %foreach.loop

foreach.end5:                                     ; preds = %foreach.loop2
  %56 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %57 = load i32, i32* %result, align 4
  ret i32 %57
}
