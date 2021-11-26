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
  %1 = load %Person*, %Person** %person, align 8
  %2 = getelementptr inbounds %Person, %Person* %1, i32 0, i32 2
  %3 = load i32, i32* %2, align 4
  %post_pp = add i32 %3, 1
  %4 = load %Person*, %Person** %person, align 8
  %5 = getelementptr inbounds %Person, %Person* %4, i32 0, i32 2
  store i32 %post_pp, i32* %5, align 4
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca %Person, align 8
  %1 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %2 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 1
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i8** %2, align 8
  %3 = getelementptr inbounds %Person, %Person* %0, i32 0, i32 2
  store i32 32, i32* %3, align 4
  %Person = load %Person, %Person* %0, align 8
  %mike = alloca %Person, align 8
  store %Person %Person, %Person* %mike, align 8
  %4 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  %5 = load i32, i32* %4, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @2, i32 0, i32 0), i32 %5)
  call void @"birthday()"(%Person* %mike)
  %7 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  %8 = load i32, i32* %7, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @3, i32 0, i32 0), i32 %8)
  %10 = load i32, i32* %result, align 4
  ret i32 %10
}
