; ModuleID = 'source.spice'
source_filename = "source.spice"

%Nested = type { i8*, i1* }
%TestStruct = type { i32*, double, %Nested* }

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@1 = private unnamed_addr constant [23 x i8] c"Field1: %d, field2: %f\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %input = alloca i32, align 4
  store i32 12, i32* %input, align 4
  %boolean = alloca i1, align 1
  store i1 true, i1* %boolean, align 1
  %0 = alloca %Nested, align 8
  %1 = getelementptr inbounds %Nested, %Nested* %0, i32 0, i32 0
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %2 = getelementptr inbounds %Nested, %Nested* %0, i32 0, i32 1
  store i1* %boolean, i1** %2, align 8
  %Nested = load %Nested, %Nested* %0, align 8
  %nestedInstance = alloca %Nested, align 8
  store %Nested %Nested, %Nested* %nestedInstance, align 8
  %3 = alloca %TestStruct, align 8
  %4 = getelementptr inbounds %TestStruct, %TestStruct* %3, i32 0, i32 0
  store i32* %input, i32** %4, align 8
  %5 = getelementptr inbounds %TestStruct, %TestStruct* %3, i32 0, i32 1
  store double 4.634000e+01, double* %5, align 8
  %6 = getelementptr inbounds %TestStruct, %TestStruct* %3, i32 0, i32 2
  store %Nested* %nestedInstance, %Nested** %6, align 8
  %TestStruct = load %TestStruct, %TestStruct* %3, align 8
  %instance = alloca %TestStruct, align 8
  store %TestStruct %TestStruct, %TestStruct* %instance, align 8
  %7 = load %TestStruct, %TestStruct* %instance, align 8
  %instance1 = alloca %TestStruct, align 8
  store %TestStruct %7, %TestStruct* %instance1, align 8
  %8 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 2
  %9 = load %Nested*, %Nested** %8, align 8
  %10 = getelementptr inbounds %Nested, %Nested* %9, i32 0, i32 1
  %11 = load i1*, i1** %10, align 8
  %12 = load i1, i1* %11, align 1
  %13 = zext i1 %12 to i32
  %14 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 1
  %15 = load double, double* %14, align 8
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @1, i32 0, i32 0), i32 %13, double %15)
  %17 = load i32, i32* %result, align 4
  ret i32 %17
}
