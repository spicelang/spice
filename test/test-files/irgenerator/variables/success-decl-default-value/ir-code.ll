; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.NestedStruct = type { i32, ptr }

@printf.str.0 = private unnamed_addr constant [18 x i8] c"Double value: %f\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [15 x i8] c"Int value: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"Short value: %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [16 x i8] c"Long value: %d\0A\00", align 1
@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@printf.str.4 = private unnamed_addr constant [18 x i8] c"String value: %s\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [16 x i8] c"Byte value: %d\0A\00", align 1
@printf.str.6 = private unnamed_addr constant [16 x i8] c"Char value: %c\0A\00", align 1
@printf.str.7 = private unnamed_addr constant [16 x i8] c"Bool value: %d\0A\00", align 1
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@printf.str.8 = private unnamed_addr constant [24 x i8] c"Struct array value: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %doubleVar = alloca double, align 8
  %intVar = alloca i32, align 4
  %shortVar = alloca i16, align 2
  %longVar = alloca i64, align 8
  %stringVar = alloca ptr, align 8
  %byteVar = alloca i8, align 1
  %charVar = alloca i8, align 1
  %boolVar = alloca i1, align 1
  %structArrayVar = alloca [4 x %struct.NestedStruct], align 8
  store i32 0, ptr %result, align 4
  store double 0.000000e+00, ptr %doubleVar, align 8
  %1 = load double, ptr %doubleVar, align 8
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %1)
  store i32 0, ptr %intVar, align 4
  %3 = load i32, ptr %intVar, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %3)
  store i16 0, ptr %shortVar, align 2
  %5 = load i16, ptr %shortVar, align 2
  %6 = sext i16 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %6)
  store i64 0, ptr %longVar, align 8
  %8 = load i64, ptr %longVar, align 8
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i64 %8)
  store ptr @0, ptr %stringVar, align 8
  %10 = load ptr, ptr %stringVar, align 8
  %11 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, ptr %10)
  store i8 0, ptr %byteVar, align 1
  %12 = load i8, ptr %byteVar, align 1
  %13 = zext i8 %12 to i32
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 %13)
  store i8 0, ptr %charVar, align 1
  %15 = load i8, ptr %charVar, align 1
  %16 = zext i8 %15 to i32
  %17 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 %16)
  store i1 false, ptr %boolVar, align 1
  %18 = load i1, ptr %boolVar, align 1
  %19 = zext i1 %18 to i32
  %20 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i32 %19)
  store [4 x %struct.NestedStruct] [%struct.NestedStruct { i32 0, ptr @1 }, %struct.NestedStruct { i32 0, ptr @1 }, %struct.NestedStruct { i32 0, ptr @1 }, %struct.NestedStruct { i32 0, ptr @1 }], ptr %structArrayVar, align 8
  %21 = getelementptr inbounds [4 x %struct.NestedStruct], ptr %structArrayVar, i64 0, i32 2
  %field2.addr = getelementptr inbounds %struct.NestedStruct, ptr %21, i64 0, i32 1
  %22 = load ptr, ptr %field2.addr, align 8
  %23 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, ptr %22)
  %24 = load i32, ptr %result, align 4
  ret i32 %24
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
