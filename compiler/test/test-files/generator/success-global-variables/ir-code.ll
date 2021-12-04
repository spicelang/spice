; ModuleID = 'source.spice'
source_filename = "source.spice"

@test1 = dso_local global i32 10
@test2 = dso_local global i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0)
@0 = private unnamed_addr constant [12 x i8] c"test string\00", align 1
@test3 = dso_local global double 5.830000e+00
@test4 = dso_local global i1 false
@1 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@2 = private unnamed_addr constant [32 x i8] c"Variable values: %d, %s, %f, %u\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 11, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  store i32 %1, i32* @test1, align 4
  %2 = alloca i8*, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @1, i32 0, i32 0), i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  store i8* %3, i8** @test2, align 8
  %4 = alloca double, align 8
  store double 5.840000e+00, double* %4, align 8
  %5 = load double, double* %4, align 8
  store double %5, double* @test3, align 8
  %6 = alloca i1, align 1
  store i1 true, i1* %6, align 1
  %7 = load i1, i1* %6, align 1
  store i1 %7, i1* @test4, align 1
  %8 = load i32, i32* @test1, align 4
  %9 = load i8*, i8** @test2, align 8
  %10 = load double, double* @test3, align 8
  %11 = load i1, i1* @test4, align 1
  %12 = zext i1 %11 to i32
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @2, i32 0, i32 0), i32 %8, i8* %9, double %10, i32 %12)
  %14 = load i32, i32* %result, align 4
  ret i32 %14
}
