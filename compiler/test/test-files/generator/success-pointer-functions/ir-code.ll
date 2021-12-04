; ModuleID = 'source.spice'
source_filename = "source.spice"

%Person = type { i8*, i8*, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@2 = private unnamed_addr constant [24 x i8] c"Age before birthday: %d\00", align 1
@3 = private unnamed_addr constant [23 x i8] c"Age after birthday: %d\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define void @"birthday()"(%Person* %0) {
entry:
  %person = alloca %Person*, align 8
  store %Person* %0, %Person** %person, align 8
  %1 = getelementptr inbounds %Person*, %Person** %person, i32 0
  %2 = load %Person*, %Person** %1, align 8
  %3 = getelementptr inbounds %Person, %Person* %2, i32 0, i32 2
  %4 = load i32, i32* %3, align 4
  %post_pp = add i32 %4, 1
  store i32 %post_pp, i32* %3, align 4
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %person = alloca %Person, align 8
  %0 = alloca i8*, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %0, align 8
  %1 = load i8*, i8** %0, align 8
  %2 = getelementptr inbounds %Person, %Person* %person, i32 0, i32 0
  store i8* %1, i8** %2, align 8
  %3 = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i8** %3, align 8
  %4 = load i8*, i8** %3, align 8
  %5 = getelementptr inbounds %Person, %Person* %person, i32 0, i32 1
  store i8* %4, i8** %5, align 8
  %6 = alloca i32, align 4
  store i32 32, i32* %6, align 4
  %7 = load i32, i32* %6, align 4
  %8 = getelementptr inbounds %Person, %Person* %person, i32 0, i32 2
  store i32 %7, i32* %8, align 4
  %9 = getelementptr inbounds %Person, %Person* %person, i32 0, i32 2
  %10 = load i32, i32* %9, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @2, i32 0, i32 0), i32 %10)
  %12 = getelementptr inbounds %Person, %Person* %person, i32 0
  %13 = alloca %Person*, align 8
  store %Person* %person, %Person** %13, align 8
  %14 = load %Person*, %Person** %13, align 8
  call void @"birthday()"(%Person* %14)
  %15 = getelementptr inbounds %Person, %Person* %person, i32 0, i32 2
  %16 = load i32, i32* %15, align 4
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @3, i32 0, i32 0), i32 %16)
  %18 = load i32, i32* %result, align 4
  ret i32 %18
}
