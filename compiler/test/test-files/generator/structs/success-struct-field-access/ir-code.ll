; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%Person = type { i8*, i8*, i32 }

@0 = private unnamed_addr constant [5 x i8] c"John\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"Doe\00", align 1
@2 = private unnamed_addr constant [15 x i8] c"John's age: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %john = alloca %Person, align 8
  %0 = alloca %Person, align 8
  %1 = alloca i8*, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %5 = load i8*, i8** %1, align 8
  %6 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 0
  store i8* %5, i8** %6, align 8
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i8** %2, align 8
  %7 = load i8*, i8** %2, align 8
  %8 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 1
  store i8* %7, i8** %8, align 8
  store i32 46, i32* %3, align 4
  %9 = load i32, i32* %3, align 4
  %10 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 2
  store i32 %9, i32* %10, align 4
  %11 = load %Person, %Person* %0, align 8
  store %Person %11, %Person* %john, align 8
  store i32 47, i32* %4, align 4
  %12 = load i32, i32* %4, align 4
  %13 = load %Person, %Person* %john, align 8
  %14 = getelementptr inbounds %Person, %Person* %john, i32 0, i32 2
  store i32 %12, i32* %14, align 4
  %15 = load %Person, %Person* %john, align 8
  %16 = getelementptr inbounds %Person, %Person* %john, i32 0, i32 2
  %17 = load i32, i32* %16, align 4
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @2, i32 0, i32 0), i32 %17)
  %19 = load i32, i32* %result, align 4
  ret i32 %19
}
