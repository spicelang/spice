; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@test1 = internal global i32 10
@test2 = internal global i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0)
@0 = private unnamed_addr constant [12 x i8] c"test string\00", align 1
@test3 = internal global double 5.830000e+00
@test4 = internal global i1 false
@1 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@2 = private unnamed_addr constant [32 x i8] c"Variable values: %d, %s, %f, %u\00", align 1

define i32 @main() {
entry.l6:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 11, i32* @test1, align 4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @1, i32 0, i32 0), i8** @test2, align 8
  store double 5.840000e+00, double* @test3, align 8
  store i1 true, i1* @test4, align 1
  %0 = load i32, i32* @test1, align 4
  %1 = load i8*, i8** @test2, align 8
  %2 = load double, double* @test3, align 8
  %3 = load i1, i1* @test4, align 1
  %4 = zext i1 %3 to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @2, i32 0, i32 0), i32 %0, i8* %1, double %2, i32 %4)
  %6 = load i32, i32* %result, align 4
  ret i32 %6
}

declare i32 @printf(i8*, ...)
