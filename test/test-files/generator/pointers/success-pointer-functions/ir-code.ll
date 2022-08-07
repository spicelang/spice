; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Person__string_string_int = type { i8*, i8*, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Person: %s, %s\0A\00", align 1
@3 = private unnamed_addr constant [25 x i8] c"Age before birthday: %d\0A\00", align 1
@4 = private unnamed_addr constant [24 x i8] c"Age after birthday: %d\0A\00", align 1

define internal void @_p__void__birthday__Personptr(%_s__Person__string_string_int* %0) {
entry.l7:
  %person = alloca %_s__Person__string_string_int*, align 8
  %1 = alloca i32, align 4
  store %_s__Person__string_string_int* %0, %_s__Person__string_string_int** %person, align 8
  %2 = load %_s__Person__string_string_int*, %_s__Person__string_string_int** %person, align 8
  %3 = load %_s__Person__string_string_int*, %_s__Person__string_string_int** %person, align 8
  %4 = getelementptr inbounds %_s__Person__string_string_int, %_s__Person__string_string_int* %3, i32 0, i32 2
  %5 = load i32, i32* %4, align 4
  %6 = add i32 %5, 1
  store i32 %6, i32* %4, align 4
  store i32 %5, i32* %1, align 4
  ret void
}

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  %0 = alloca %_s__Person__string_string_int, align 8
  %1 = alloca %_s__Person__string_string_int*, align 8
  %2 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  %3 = getelementptr inbounds %_s__Person__string_string_int, %_s__Person__string_string_int* %0, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %3, align 8
  %4 = getelementptr inbounds %_s__Person__string_string_int, %_s__Person__string_string_int* %0, i32 0, i32 1
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i8** %4, align 8
  %5 = getelementptr inbounds %_s__Person__string_string_int, %_s__Person__string_string_int* %0, i32 0, i32 2
  store i32 32, i32* %5, align 4
  %6 = load %_s__Person__string_string_int, %_s__Person__string_string_int* %0, align 8
  %7 = getelementptr inbounds %_s__Person__string_string_int, %_s__Person__string_string_int* %0, i32 0, i32 1
  %8 = load i8*, i8** %7, align 8
  %9 = load %_s__Person__string_string_int, %_s__Person__string_string_int* %0, align 8
  %10 = getelementptr inbounds %_s__Person__string_string_int, %_s__Person__string_string_int* %0, i32 0, i32 0
  %11 = load i8*, i8** %10, align 8
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @2, i32 0, i32 0), i8* %8, i8* %11)
  %13 = load %_s__Person__string_string_int, %_s__Person__string_string_int* %0, align 8
  %14 = getelementptr inbounds %_s__Person__string_string_int, %_s__Person__string_string_int* %0, i32 0, i32 2
  %15 = load i32, i32* %14, align 4
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @3, i32 0, i32 0), i32 %15)
  store %_s__Person__string_string_int* %0, %_s__Person__string_string_int** %1, align 8
  %17 = load %_s__Person__string_string_int*, %_s__Person__string_string_int** %1, align 8
  call void @_p__void__birthday__Personptr(%_s__Person__string_string_int* %17)
  store i1 true, i1* %2, align 1
  %18 = load %_s__Person__string_string_int, %_s__Person__string_string_int* %0, align 8
  %19 = getelementptr inbounds %_s__Person__string_string_int, %_s__Person__string_string_int* %0, i32 0, i32 2
  %20 = load i32, i32* %19, align 4
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @4, i32 0, i32 0), i32 %20)
  %22 = load i32, i32* %result, align 4
  ret i32 %22
}

declare i32 @printf(i8*, ...)
