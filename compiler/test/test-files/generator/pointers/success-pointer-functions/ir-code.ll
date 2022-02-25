; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%Person = type { i8*, i8*, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Person: %s, %s\0A\00", align 1
@3 = private unnamed_addr constant [25 x i8] c"Age before birthday: %d\0A\00", align 1
@4 = private unnamed_addr constant [24 x i8] c"Age after birthday: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define internal void @"birthday(struct(Person)*)"(%Person* %0) #0 {
entry:
  %person = alloca %Person*, align 8
  %1 = alloca i32, align 4
  store %Person* %0, %Person** %person, align 8
  %2 = load %Person*, %Person** %person, align 8
  %3 = load %Person*, %Person** %person, align 8
  %4 = getelementptr inbounds %Person, %Person* %3, i32 0, i32 2
  %5 = load i32, i32* %4, align 4
  %6 = add i32 %5, 1
  store i32 %6, i32* %4, align 4
  store i32 %5, i32* %1, align 4
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %mike = alloca %Person, align 8
  %0 = alloca %Person, align 8
  %1 = alloca i8*, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  %4 = alloca %Person*, align 8
  %5 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %6 = load i8*, i8** %1, align 8
  %7 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 0
  store i8* %6, i8** %7, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i8** %2, align 8
  %8 = load i8*, i8** %2, align 8
  %9 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 1
  store i8* %8, i8** %9, align 8
  store i32 32, i32* %3, align 4
  %10 = load i32, i32* %3, align 4
  %11 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 2
  store i32 %10, i32* %11, align 4
  %12 = load %Person, %Person* %0, align 8
  store %Person %12, %Person* %mike, align 8
  %13 = load %Person, %Person* %mike, align 8
  %14 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 1
  %15 = load i8*, i8** %14, align 8
  %16 = load %Person, %Person* %mike, align 8
  %17 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 0
  %18 = load i8*, i8** %17, align 8
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @2, i32 0, i32 0), i8* %15, i8* %18)
  %20 = load %Person, %Person* %mike, align 8
  %21 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  %22 = load i32, i32* %21, align 4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @3, i32 0, i32 0), i32 %22)
  %24 = load %Person, %Person* %mike, align 8
  store %Person* %mike, %Person** %4, align 8
  store %Person* %mike, %Person** %4, align 8
  %25 = load %Person*, %Person** %4, align 8
  call void @"birthday(struct(Person)*)"(%Person* %25)
  store i1 true, i1* %5, align 1
  %26 = load %Person, %Person* %mike, align 8
  %27 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  %28 = load i32, i32* %27, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @4, i32 0, i32 0), i32 %28)
  %30 = load i32, i32* %result, align 4
  ret i32 %30
}

attributes #0 = { nounwind }
