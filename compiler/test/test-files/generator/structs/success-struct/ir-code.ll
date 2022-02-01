; ModuleID = 'source.spice'
source_filename = "source.spice"

%Nested = type { i8*, i1* }
%TestStruct = type { i32*, double, %Nested* }

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@1 = private unnamed_addr constant [24 x i8] c"Field1: %d, field2: %f\0A\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Output: %s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %0 = alloca i32, align 4
  %boolean = alloca i1, align 1
  %1 = alloca i1, align 1
  %nestedInstance = alloca %Nested, align 8
  %Nested = alloca %Nested, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i1*, align 8
  %instance = alloca %TestStruct, align 8
  %TestStruct = alloca %TestStruct, align 8
  %4 = alloca i32*, align 8
  %5 = alloca double, align 8
  %6 = alloca %Nested*, align 8
  %instance1 = alloca %TestStruct, align 8
  store i32 0, i32* %result, align 4
  store i32 12, i32* %0, align 4
  %7 = load i32, i32* %0, align 4
  store i32 %7, i32* %input, align 4
  store i1 true, i1* %1, align 1
  %8 = load i1, i1* %1, align 1
  store i1 %8, i1* %boolean, align 1
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %2, align 8
  %9 = load i8*, i8** %2, align 8
  %10 = getelementptr inbounds %Nested, %Nested* %Nested, i32 0, i32 0
  store i8* %9, i8** %10, align 8
  %11 = load i1, i1* %boolean, align 1
  store i1* %boolean, i1** %3, align 8
  store i1* %boolean, i1** %3, align 8
  %12 = load i1*, i1** %3, align 8
  %13 = getelementptr inbounds %Nested, %Nested* %Nested, i32 0, i32 1
  store i1* %12, i1** %13, align 8
  %14 = load %Nested, %Nested* %Nested, align 8
  store %Nested %14, %Nested* %nestedInstance, align 8
  %15 = load i32, i32* %input, align 4
  store i32* %input, i32** %4, align 8
  store i32* %input, i32** %4, align 8
  %16 = load i32*, i32** %4, align 8
  %17 = getelementptr inbounds %TestStruct, %TestStruct* %TestStruct, i32 0, i32 0
  store i32* %16, i32** %17, align 8
  store double 4.634000e+01, double* %5, align 8
  %18 = load double, double* %5, align 8
  %19 = getelementptr inbounds %TestStruct, %TestStruct* %TestStruct, i32 0, i32 1
  store double %18, double* %19, align 8
  %20 = load %Nested, %Nested* %nestedInstance, align 8
  store %Nested* %nestedInstance, %Nested** %6, align 8
  store %Nested* %nestedInstance, %Nested** %6, align 8
  %21 = load %Nested*, %Nested** %6, align 8
  %22 = getelementptr inbounds %TestStruct, %TestStruct* %TestStruct, i32 0, i32 2
  store %Nested* %21, %Nested** %22, align 8
  %23 = load %TestStruct, %TestStruct* %TestStruct, align 8
  store %TestStruct %23, %TestStruct* %instance, align 8
  %24 = load %TestStruct, %TestStruct* %instance, align 8
  store %TestStruct %24, %TestStruct* %instance1, align 8
  %25 = load %TestStruct, %TestStruct* %instance, align 8
  %26 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 2
  %27 = load %Nested*, %Nested** %26, align 8
  %28 = load %Nested*, %Nested** %22, align 8
  %29 = getelementptr inbounds %Nested, %Nested* %27, i32 0, i32 1
  %30 = load i1*, i1** %29, align 8
  %31 = load i1, i1* %30, align 1
  store i1 %31, i1* %30, align 1
  %32 = load i1, i1* %30, align 1
  %33 = zext i1 %32 to i32
  %34 = load %TestStruct, %TestStruct* %instance1, align 8
  %35 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 1
  %36 = load double, double* %35, align 8
  %37 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %33, double %36)
  %38 = load %TestStruct, %TestStruct* %instance1, align 8
  %39 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 2
  %40 = load %Nested*, %Nested** %39, align 8
  %41 = load %Nested*, %Nested** %22, align 8
  %42 = getelementptr inbounds %Nested, %Nested* %40, i32 0, i32 0
  %43 = load i8*, i8** %42, align 8
  %44 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0), i8* %43)
  %45 = load i32, i32* %result, align 4
  ret i32 %45
}
