; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s_Person_string_string_int = type { i8*, i8*, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Person: %s, %s\0A\00", align 1
@3 = private unnamed_addr constant [25 x i8] c"Age before birthday: %d\0A\00", align 1
@4 = private unnamed_addr constant [24 x i8] c"Age after birthday: %d\0A\00", align 1

; Function Attrs: nounwind
define internal void @"_p_birthday_struct(Person)*"(%_s_Person_string_string_int* %0) #0 {
entry:
  %person = alloca %_s_Person_string_string_int*, align 8
  %1 = alloca i32, align 4
  store %_s_Person_string_string_int* %0, %_s_Person_string_string_int** %person, align 8
  %2 = load %_s_Person_string_string_int*, %_s_Person_string_string_int** %person, align 8
  %3 = load %_s_Person_string_string_int*, %_s_Person_string_string_int** %person, align 8
  %4 = getelementptr inbounds %_s_Person_string_string_int, %_s_Person_string_string_int* %3, i32 0, i32 2
  %5 = load i32, i32* %4, align 4
  %6 = add i32 %5, 1
  store i32 %6, i32* %4, align 4
  store i32 %5, i32* %1, align 4
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %mike = alloca %_s_Person_string_string_int, align 8
  %0 = alloca %_s_Person_string_string_int, align 8
  %1 = alloca i8*, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  %4 = alloca %_s_Person_string_string_int*, align 8
  %5 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %6 = load i8*, i8** %1, align 8
  %7 = getelementptr inbounds %_s_Person_string_string_int, %_s_Person_string_string_int* %0, i32 0, i32 0
  store i8* %6, i8** %7, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i8** %2, align 8
  %8 = load i8*, i8** %2, align 8
  %9 = getelementptr inbounds %_s_Person_string_string_int, %_s_Person_string_string_int* %0, i32 0, i32 1
  store i8* %8, i8** %9, align 8
  store i32 32, i32* %3, align 4
  %10 = load i32, i32* %3, align 4
  %11 = getelementptr inbounds %_s_Person_string_string_int, %_s_Person_string_string_int* %0, i32 0, i32 2
  store i32 %10, i32* %11, align 4
  %12 = load %_s_Person_string_string_int, %_s_Person_string_string_int* %0, align 8
  store %_s_Person_string_string_int %12, %_s_Person_string_string_int* %mike, align 8
  %13 = load %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, align 8
  %14 = getelementptr inbounds %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, i32 0, i32 1
  %15 = load i8*, i8** %14, align 8
  %16 = load %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, align 8
  %17 = getelementptr inbounds %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, i32 0, i32 0
  %18 = load i8*, i8** %17, align 8
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @2, i32 0, i32 0), i8* %15, i8* %18)
  %20 = load %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, align 8
  %21 = getelementptr inbounds %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, i32 0, i32 2
  %22 = load i32, i32* %21, align 4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @3, i32 0, i32 0), i32 %22)
  %24 = load %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, align 8
  store %_s_Person_string_string_int* %mike, %_s_Person_string_string_int** %4, align 8
  store %_s_Person_string_string_int* %mike, %_s_Person_string_string_int** %4, align 8
  %25 = load %_s_Person_string_string_int*, %_s_Person_string_string_int** %4, align 8
  call void @"_p_birthday_struct(Person)*"(%_s_Person_string_string_int* %25)
  store i1 true, i1* %5, align 1
  %26 = load %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, align 8
  %27 = getelementptr inbounds %_s_Person_string_string_int, %_s_Person_string_string_int* %mike, i32 0, i32 2
  %28 = load i32, i32* %27, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @4, i32 0, i32 0), i32 %28)
  %30 = load i32, i32* %result, align 4
  ret i32 %30
}

declare i32 @printf(i8*, ...)

attributes #0 = { nounwind }
