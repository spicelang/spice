; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@2 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@3 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@4 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@5 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@6 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@7 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@8 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@9 = private unnamed_addr constant [5 x i8] c"Hi! \00", align 1
@10 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@11 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@12 = private unnamed_addr constant [2 x i8] c" \00", align 1
@13 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@14 = private unnamed_addr constant [8 x i8] c"Prefix \00", align 1
@15 = private unnamed_addr constant [8 x i8] c" Suffix\00", align 1
@16 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@17 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@18 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@19 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@20 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@21 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@22 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@23 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@24 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@25 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@26 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@27 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@28 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@29 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@30 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@31 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@32 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@33 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@34 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@35 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@36 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@37 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@38 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@39 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@40 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@41 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@42 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@43 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@44 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@45 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@46 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@47 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@48 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %0 = alloca %_s__String__charptr_long_long, align 8
  %1 = alloca %_s__String__charptr_long_long, align 8
  %2 = alloca %_s__String__charptr_long_long, align 8
  %3 = alloca %_s__String__charptr_long_long, align 8
  %4 = alloca %_s__String__charptr_long_long, align 8
  %5 = alloca %_s__String__charptr_long_long, align 8
  %6 = alloca %_s__String__charptr_long_long, align 8
  %7 = alloca %_s__String__charptr_long_long, align 8
  %8 = alloca %_s__String__charptr_long_long, align 8
  %9 = alloca %_s__String__charptr_long_long, align 8
  %10 = alloca %_s__String__charptr_long_long, align 8
  %11 = alloca %_s__String__charptr_long_long, align 8
  %12 = alloca %_s__String__charptr_long_long, align 8
  %13 = alloca %_s__String__charptr_long_long, align 8
  %14 = alloca %_s__String__charptr_long_long, align 8
  %15 = alloca %_s__String__charptr_long_long, align 8
  %16 = alloca %_s__String__charptr_long_long, align 8
  %17 = alloca %_s__String__charptr_long_long, align 8
  %18 = alloca %_s__String__charptr_long_long, align 8
  %19 = alloca %_s__String__charptr_long_long, align 8
  %20 = alloca %_s__String__charptr_long_long, align 8
  %21 = alloca i16, align 2
  %22 = alloca ptr, align 8
  %23 = alloca %_s__String__charptr_long_long, align 8
  %s2 = alloca ptr, align 8
  %s21 = alloca i32, align 4
  %24 = alloca %_s__String__charptr_long_long, align 8
  %25 = alloca i32, align 4
  %26 = alloca i8, align 1
  %27 = alloca %_s__String__charptr_long_long, align 8
  %s3 = alloca i32, align 4
  %s32 = alloca i8, align 1
  %28 = alloca %_s__String__charptr_long_long, align 8
  %s33 = alloca i32, align 4
  %29 = alloca i1, align 1
  %30 = alloca i1, align 1
  %31 = alloca i1, align 1
  %32 = alloca i1, align 1
  %33 = alloca i1, align 1
  %34 = alloca i1, align 1
  %s4 = alloca ptr, align 8
  %35 = alloca %_s__String__charptr_long_long, align 8
  %s5 = alloca ptr, align 8
  %36 = alloca %_s__String__charptr_long_long, align 8
  %s6 = alloca ptr, align 8
  %37 = alloca %_s__String__charptr_long_long, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__String__void__ctor__string_string(ptr %0, ptr @1, ptr @2)
  %38 = load %_s__String__charptr_long_long, ptr %0, align 8
  store %_s__String__charptr_long_long %38, ptr %1, align 8
  %39 = call ptr @_mf__String__string__getRaw(ptr %1)
  %40 = call i32 (ptr, ...) @printf(ptr @0, ptr %39)
  call void @_mp__String__void__ctor__string_string(ptr %2, ptr @3, ptr @4)
  %41 = load %_s__String__charptr_long_long, ptr %2, align 8
  store %_s__String__charptr_long_long %41, ptr %3, align 8
  %42 = call ptr @_mf__String__string__getRaw(ptr %3)
  %43 = call i32 (ptr, ...) @printf(ptr @5, ptr %42)
  %44 = load %_s__String__charptr_long_long, ptr %3, align 8
  store %_s__String__charptr_long_long %44, ptr %4, align 8
  %45 = call ptr @_mf__String__string__getRaw(ptr %4)
  call void @_mp__String__void__ctor__string_string(ptr %5, ptr %45, ptr @7)
  %46 = load %_s__String__charptr_long_long, ptr %5, align 8
  store %_s__String__charptr_long_long %46, ptr %6, align 8
  %47 = call ptr @_mf__String__string__getRaw(ptr %6)
  %48 = call i32 (ptr, ...) @printf(ptr @6, ptr %47)
  %49 = call ptr @_mf__String__string__getRaw(ptr %3)
  call void @_mp__String__void__ctor__string_string(ptr %7, ptr @9, ptr %49)
  %50 = load %_s__String__charptr_long_long, ptr %7, align 8
  store %_s__String__charptr_long_long %50, ptr %8, align 8
  %51 = call ptr @_mf__String__string__getRaw(ptr %8)
  %52 = call i32 (ptr, ...) @printf(ptr @8, ptr %51)
  %53 = load %_s__String__charptr_long_long, ptr %3, align 8
  store %_s__String__charptr_long_long %53, ptr %9, align 8
  %54 = call ptr @_mf__String__string__getRaw(ptr %9)
  %55 = call ptr @_mf__String__string__getRaw(ptr %3)
  call void @_mp__String__void__ctor__string_string(ptr %10, ptr %54, ptr %55)
  %56 = load %_s__String__charptr_long_long, ptr %10, align 8
  store %_s__String__charptr_long_long %56, ptr %11, align 8
  %57 = call ptr @_mf__String__string__getRaw(ptr %11)
  %58 = call i32 (ptr, ...) @printf(ptr @10, ptr %57)
  %59 = load %_s__String__charptr_long_long, ptr %3, align 8
  store %_s__String__charptr_long_long %59, ptr %12, align 8
  %60 = call ptr @_mf__String__string__getRaw(ptr %12)
  call void @_mp__String__void__ctor__string_string(ptr %13, ptr %60, ptr @12)
  %61 = load %_s__String__charptr_long_long, ptr %13, align 8
  store %_s__String__charptr_long_long %61, ptr %14, align 8
  %62 = call ptr @_mf__String__string__getRaw(ptr %14)
  %63 = call ptr @_mf__String__string__getRaw(ptr %3)
  call void @_mp__String__void__ctor__string_string(ptr %15, ptr %62, ptr %63)
  %64 = load %_s__String__charptr_long_long, ptr %15, align 8
  store %_s__String__charptr_long_long %64, ptr %16, align 8
  %65 = call ptr @_mf__String__string__getRaw(ptr %16)
  %66 = call i32 (ptr, ...) @printf(ptr @11, ptr %65)
  %67 = call ptr @_mf__String__string__getRaw(ptr %3)
  call void @_mp__String__void__ctor__string_string(ptr %17, ptr @14, ptr %67)
  %68 = load %_s__String__charptr_long_long, ptr %17, align 8
  store %_s__String__charptr_long_long %68, ptr %18, align 8
  %69 = call ptr @_mf__String__string__getRaw(ptr %18)
  call void @_mp__String__void__ctor__string_string(ptr %19, ptr %69, ptr @15)
  %70 = load %_s__String__charptr_long_long, ptr %19, align 8
  store %_s__String__charptr_long_long %70, ptr %20, align 8
  %71 = call ptr @_mf__String__string__getRaw(ptr %20)
  %72 = call i32 (ptr, ...) @printf(ptr @13, ptr %71)
  store i16 4, ptr %21, align 2
  store ptr @18, ptr %22, align 8
  call void @_mp__String__void__ctor__string(ptr %23, ptr @17)
  call void @_mp__String__void__opMul__short(ptr %23, i16 4)
  %73 = load %_s__String__charptr_long_long, ptr %23, align 8
  store %_s__String__charptr_long_long %73, ptr %23, align 8
  %74 = call ptr @_mf__String__string__getRaw(ptr %23)
  %75 = call i32 (ptr, ...) @printf(ptr @16, ptr %74)
  store ptr @20, ptr %s2, align 8
  store i32 5, ptr %s21, align 4
  call void @_mp__String__void__ctor__string(ptr %24, ptr @19)
  call void @_mp__String__void__opMul__int(ptr %24, i32 5)
  %76 = load %_s__String__charptr_long_long, ptr %24, align 8
  store %_s__String__charptr_long_long %76, ptr %24, align 8
  %77 = call ptr @_mf__String__string__getRaw(ptr %24)
  %78 = call i32 (ptr, ...) @printf(ptr @21, ptr %77)
  store i32 20, ptr %25, align 4
  store i8 97, ptr %26, align 1
  call void @_mp__String__void__ctor__char(ptr %27, i8 97)
  call void @_mp__String__void__opMul__int(ptr %27, i32 20)
  %79 = load %_s__String__charptr_long_long, ptr %27, align 8
  store %_s__String__charptr_long_long %79, ptr %27, align 8
  %80 = call ptr @_mf__String__string__getRaw(ptr %27)
  %81 = call i32 (ptr, ...) @printf(ptr @22, ptr %80)
  store i32 2, ptr %s3, align 4
  store i8 99, ptr %s32, align 1
  call void @_mp__String__void__ctor__char(ptr %28, i8 99)
  call void @_mp__String__void__opMul__int(ptr %28, i32 2)
  %82 = load %_s__String__charptr_long_long, ptr %28, align 8
  store i32 7, ptr %s33, align 4
  call void @_mp__String__void__opMul__int(ptr %28, i32 7)
  %83 = load %_s__String__charptr_long_long, ptr %28, align 8
  store %_s__String__charptr_long_long %83, ptr %28, align 8
  %84 = call ptr @_mf__String__string__getRaw(ptr %28)
  %85 = call i32 (ptr, ...) @printf(ptr @23, ptr %84)
  %86 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @25, ptr @26)
  store i1 %86, ptr %29, align 1
  %87 = load i1, ptr %29, align 1
  %88 = zext i1 %87 to i32
  %89 = call i32 (ptr, ...) @printf(ptr @24, i32 %88)
  %90 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @28, ptr @29)
  store i1 %90, ptr %30, align 1
  %91 = load i1, ptr %30, align 1
  %92 = zext i1 %91 to i32
  %93 = call i32 (ptr, ...) @printf(ptr @27, i32 %92)
  %94 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @31, ptr @32)
  store i1 %94, ptr %31, align 1
  %95 = load i1, ptr %31, align 1
  %96 = zext i1 %95 to i32
  %97 = call i32 (ptr, ...) @printf(ptr @30, i32 %96)
  %98 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @34, ptr @35)
  %99 = xor i1 %98, true
  store i1 %99, ptr %32, align 1
  %100 = load i1, ptr %32, align 1
  %101 = zext i1 %100 to i32
  %102 = call i32 (ptr, ...) @printf(ptr @33, i32 %101)
  %103 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @37, ptr @38)
  %104 = xor i1 %103, true
  store i1 %104, ptr %33, align 1
  %105 = load i1, ptr %33, align 1
  %106 = zext i1 %105 to i32
  %107 = call i32 (ptr, ...) @printf(ptr @36, i32 %106)
  %108 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @40, ptr @41)
  %109 = xor i1 %108, true
  store i1 %109, ptr %34, align 1
  %110 = load i1, ptr %34, align 1
  %111 = zext i1 %110 to i32
  %112 = call i32 (ptr, ...) @printf(ptr @39, i32 %111)
  store ptr @42, ptr %s4, align 8
  %113 = load ptr, ptr %s4, align 8
  call void @_mp__String__void__ctor__string(ptr %35, ptr %113)
  call void @_mp__String__void__append__char(ptr %35, i8 108)
  %114 = call ptr @_mf__String__string__getRaw(ptr %35)
  %115 = call i32 (ptr, ...) @printf(ptr @43, ptr %114)
  store ptr @44, ptr %s5, align 8
  %116 = load ptr, ptr %s5, align 8
  call void @_mp__String__void__ctor__string(ptr %36, ptr %116)
  call void @_mp__String__void__append__string(ptr %36, ptr @45)
  %117 = call ptr @_mf__String__string__getRaw(ptr %36)
  %118 = call i32 (ptr, ...) @printf(ptr @46, ptr %117)
  store ptr @47, ptr %s6, align 8
  %119 = load ptr, ptr %s6, align 8
  call void @_mp__String__void__ctor__string(ptr %37, ptr %119)
  call void @_mp__String__void__opMul__int(ptr %37, i32 3)
  %120 = call ptr @_mf__String__string__getRaw(ptr %37)
  %121 = call i32 (ptr, ...) @printf(ptr @48, ptr %120)
  br label %cleanup.l1

cleanup.l1:                                       ; preds = %entry.l1
  call void @_mp__String__dtor(ptr %17)
  call void @_mp__String__dtor(ptr %19)
  call void @_mp__String__dtor(ptr %23)
  call void @_mp__String__dtor(ptr %24)
  call void @_mp__String__dtor(ptr %27)
  call void @_mp__String__dtor(ptr %28)
  call void @_mp__String__dtor(ptr %35)
  call void @_mp__String__dtor(ptr %36)
  call void @_mp__String__dtor(ptr %0)
  call void @_mp__String__dtor(ptr %37)
  call void @_mp__String__dtor(ptr %2)
  call void @_mp__String__dtor(ptr %5)
  call void @_mp__String__dtor(ptr %7)
  call void @_mp__String__dtor(ptr %10)
  call void @_mp__String__dtor(ptr %13)
  call void @_mp__String__dtor(ptr %15)
  %122 = load i32, ptr %result, align 4
  ret i32 %122
}

declare i32 @printf(ptr, ...)

declare void @_mp__String__void__ctor__string_string(ptr, ptr, ptr)

declare ptr @_mf__String__string__getRaw(ptr)

declare void @_mp__String__void__ctor__string(ptr, ptr)

declare void @_mp__String__void__opMul__short(ptr, i16)

declare void @_mp__String__void__opMul__int(ptr, i32)

declare void @_mp__String__void__ctor__char(ptr, i8)

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr)

declare void @_mp__String__void__append__char(ptr, i8)

declare void @_mp__String__void__append__string(ptr, ptr)

declare void @_mp__String__dtor(ptr)
