; ModuleID = 'source.spice'
source_filename = "source.spice"

%Nested = type { i8*, i1* }
%TestStruct = type { i32*, double, %Nested* }

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@1 = private unnamed_addr constant [24 x i8] c"Field1: %d, field2: %f\0A\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Output: %s, %p\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i1, align 1
  %Nested = alloca %Nested, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i1*, align 8
  %TestStruct = alloca %TestStruct, align 8
  %4 = alloca i32*, align 8
  %5 = alloca double, align 8
  %6 = alloca %Nested*, align 8
  store i32 0, i32* %result, align 4
  store i32 12, i32* %0, align 4
  store i1 true, i1* %1, align 1
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %2, align 8
  %7 = load i8*, i8** %2, align 8
  %8 = getelementptr inbounds %Nested, %Nested* %Nested, i32 0, i32 0
  store i8* %7, i8** %8, align 8
  %9 = load i1, i1* %1, align 1
  store i1* %1, i1** %3, align 8
  store i1* %1, i1** %3, align 8
  %10 = load i1*, i1** %3, align 8
  %11 = getelementptr inbounds %Nested, %Nested* %Nested, i32 0, i32 1
  store i1* %10, i1** %11, align 8
  %12 = load i32, i32* %0, align 4
  store i32* %0, i32** %4, align 8
  store i32* %0, i32** %4, align 8
  %13 = load i32*, i32** %4, align 8
  %14 = getelementptr inbounds %TestStruct, %TestStruct* %TestStruct, i32 0, i32 0
  store i32* %13, i32** %14, align 8
  store double 4.634000e+01, double* %5, align 8
  %15 = load double, double* %5, align 8
  %16 = getelementptr inbounds %TestStruct, %TestStruct* %TestStruct, i32 0, i32 1
  store double %15, double* %16, align 8
  %17 = load %Nested, %Nested* %Nested, align 8
  store %Nested* %Nested, %Nested** %6, align 8
  store %Nested* %Nested, %Nested** %6, align 8
  %18 = load %Nested*, %Nested** %6, align 8
  %19 = getelementptr inbounds %TestStruct, %TestStruct* %TestStruct, i32 0, i32 2
  store %Nested* %18, %Nested** %19, align 8
  %20 = load %TestStruct, %TestStruct* %TestStruct, align 8
  %21 = getelementptr %TestStruct, %TestStruct* %TestStruct, i32 0, i32 2
  %22 = load %Nested*, %Nested** %21, align 8
  %23 = load %Nested*, %Nested** %19, align 8
  %24 = getelementptr %Nested, %Nested* %22, i32 0, i32 1
  %25 = load i1*, i1** %24, align 8
  %26 = load i1, i1* %25, align 1
  store i1 %26, i1* %25, align 1
  %27 = load i1, i1* %25, align 1
  %28 = zext i1 %27 to i32
  %29 = load %TestStruct, %TestStruct* %TestStruct, align 8
  %30 = getelementptr %TestStruct, %TestStruct* %TestStruct, i32 0, i32 1
  %31 = load double, double* %30, align 8
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %28, double %31)
  %33 = load %TestStruct, %TestStruct* %TestStruct, align 8
  %34 = getelementptr %TestStruct, %TestStruct* %TestStruct, i32 0, i32 2
  %35 = load %Nested*, %Nested** %34, align 8
  %36 = load %Nested*, %Nested** %19, align 8
  %37 = getelementptr %Nested, %Nested* %35, i32 0, i32 0
  %38 = load i8*, i8** %37, align 8
  %39 = load %TestStruct, %TestStruct* %TestStruct, align 8
  %40 = getelementptr %TestStruct, %TestStruct* %TestStruct, i32 0, i32 0
  %41 = load i32*, i32** %40, align 8
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @2, i32 0, i32 0), i8* %38, i32* %41)
  %43 = load i32, i32* %result, align 4
  ret i32 %43
}
