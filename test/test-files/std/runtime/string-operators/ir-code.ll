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
@24 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@25 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@26 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@27 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@28 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@29 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@30 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@31 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@32 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@33 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@34 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@35 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@36 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@37 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@38 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@39 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@40 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@41 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@42 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@43 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@44 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@45 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@46 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@47 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@48 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@49 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@50 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@51 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@52 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@53 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@54 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@55 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@56 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@57 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@58 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@59 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@60 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@61 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@62 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@63 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@64 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@65 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@66 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %0 = alloca %_s__String__charptr_long_long, align 8
  %1 = alloca ptr, align 8
  %2 = alloca %_s__String__charptr_long_long, align 8
  %3 = alloca ptr, align 8
  %4 = alloca %_s__String__charptr_long_long, align 8
  %5 = alloca %_s__String__charptr_long_long, align 8
  %s1 = alloca %_s__String__charptr_long_long, align 8
  %s11 = alloca ptr, align 8
  %s12 = alloca %_s__String__charptr_long_long, align 8
  %s13 = alloca ptr, align 8
  %6 = alloca %_s__String__charptr_long_long, align 8
  %7 = alloca %_s__String__charptr_long_long, align 8
  %8 = alloca %_s__String__charptr_long_long, align 8
  %9 = alloca %_s__String__charptr_long_long, align 8
  %10 = alloca %_s__String__charptr_long_long, align 8
  %11 = alloca ptr, align 8
  %12 = alloca %_s__String__charptr_long_long, align 8
  %13 = alloca %_s__String__charptr_long_long, align 8
  %14 = alloca %_s__String__charptr_long_long, align 8
  %15 = alloca %_s__String__charptr_long_long, align 8
  %16 = alloca %_s__String__charptr_long_long, align 8
  %17 = alloca %_s__String__charptr_long_long, align 8
  %18 = alloca %_s__String__charptr_long_long, align 8
  %19 = alloca %_s__String__charptr_long_long, align 8
  %20 = alloca ptr, align 8
  %21 = alloca %_s__String__charptr_long_long, align 8
  %22 = alloca %_s__String__charptr_long_long, align 8
  %23 = alloca %_s__String__charptr_long_long, align 8
  %24 = alloca %_s__String__charptr_long_long, align 8
  %25 = alloca ptr, align 8
  %26 = alloca i16, align 2
  %27 = alloca %_s__String__charptr_long_long, align 8
  %28 = alloca ptr, align 8
  %29 = alloca %_s__String__charptr_long_long, align 8
  %s2 = alloca %_s__String__charptr_long_long, align 8
  %s24 = alloca ptr, align 8
  %s25 = alloca %_s__String__charptr_long_long, align 8
  %s26 = alloca ptr, align 8
  %s27 = alloca i32, align 4
  %30 = alloca %_s__String__charptr_long_long, align 8
  %31 = alloca %_s__String__charptr_long_long, align 8
  %32 = alloca i8, align 1
  %33 = alloca i32, align 4
  %34 = alloca %_s__String__charptr_long_long, align 8
  %35 = alloca i8, align 1
  %36 = alloca %_s__String__charptr_long_long, align 8
  %s3 = alloca %_s__String__charptr_long_long, align 8
  %s38 = alloca i8, align 1
  %s39 = alloca i32, align 4
  %s310 = alloca %_s__String__charptr_long_long, align 8
  %s311 = alloca i8, align 1
  %37 = alloca %_s__String__charptr_long_long, align 8
  %s312 = alloca i32, align 4
  %38 = alloca %_s__String__charptr_long_long, align 8
  %39 = alloca ptr, align 8
  %40 = alloca i1, align 1
  %41 = alloca ptr, align 8
  %42 = alloca i1, align 1
  %43 = alloca ptr, align 8
  %44 = alloca i1, align 1
  %45 = alloca %_s__String__charptr_long_long, align 8
  %46 = alloca ptr, align 8
  %47 = alloca %_s__String__charptr_long_long, align 8
  %48 = alloca ptr, align 8
  %49 = alloca i1, align 1
  %50 = alloca %_s__String__charptr_long_long, align 8
  %51 = alloca ptr, align 8
  %52 = alloca %_s__String__charptr_long_long, align 8
  %53 = alloca ptr, align 8
  %54 = alloca i1, align 1
  %55 = alloca %_s__String__charptr_long_long, align 8
  %56 = alloca ptr, align 8
  %57 = alloca %_s__String__charptr_long_long, align 8
  %58 = alloca ptr, align 8
  %59 = alloca i1, align 1
  %60 = alloca ptr, align 8
  %61 = alloca i1, align 1
  %62 = alloca ptr, align 8
  %63 = alloca i1, align 1
  %64 = alloca ptr, align 8
  %65 = alloca i1, align 1
  %66 = alloca %_s__String__charptr_long_long, align 8
  %67 = alloca ptr, align 8
  %68 = alloca %_s__String__charptr_long_long, align 8
  %69 = alloca ptr, align 8
  %70 = alloca i1, align 1
  %71 = alloca %_s__String__charptr_long_long, align 8
  %72 = alloca ptr, align 8
  %73 = alloca %_s__String__charptr_long_long, align 8
  %74 = alloca ptr, align 8
  %75 = alloca i1, align 1
  %76 = alloca %_s__String__charptr_long_long, align 8
  %77 = alloca ptr, align 8
  %78 = alloca %_s__String__charptr_long_long, align 8
  %79 = alloca ptr, align 8
  %80 = alloca i1, align 1
  %s4 = alloca %_s__String__charptr_long_long, align 8
  %s413 = alloca ptr, align 8
  %s5 = alloca %_s__String__charptr_long_long, align 8
  %s514 = alloca ptr, align 8
  %s6 = alloca %_s__String__charptr_long_long, align 8
  %s615 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store ptr @1, ptr %1, align 8
  %81 = load ptr, ptr %1, align 8
  call void @_mp__String__void__ctor__string(ptr %0, ptr %81)
  %82 = load %_s__String__charptr_long_long, ptr %0, align 8
  store ptr @2, ptr %3, align 8
  %83 = load ptr, ptr %3, align 8
  call void @_mp__String__void__ctor__string(ptr %2, ptr %83)
  %84 = load %_s__String__charptr_long_long, ptr %2, align 8
  call void @_mp__String__void__ctor__String_String(ptr %4, %_s__String__charptr_long_long %82, %_s__String__charptr_long_long %84)
  %85 = load %_s__String__charptr_long_long, ptr %4, align 8
  store %_s__String__charptr_long_long %85, ptr %5, align 8
  %86 = call ptr @_mf__String__charptr__getRaw(ptr %5)
  %87 = call i32 (ptr, ...) @printf(ptr @0, ptr %86)
  store ptr @3, ptr %s11, align 8
  %88 = load ptr, ptr %s11, align 8
  call void @_mp__String__void__ctor__string(ptr %s1, ptr %88)
  %89 = load %_s__String__charptr_long_long, ptr %s1, align 8
  store ptr @4, ptr %s13, align 8
  %90 = load ptr, ptr %s13, align 8
  call void @_mp__String__void__ctor__string(ptr %s12, ptr %90)
  %91 = load %_s__String__charptr_long_long, ptr %s12, align 8
  call void @_mp__String__void__ctor__String_String(ptr %6, %_s__String__charptr_long_long %89, %_s__String__charptr_long_long %91)
  %92 = load %_s__String__charptr_long_long, ptr %6, align 8
  store %_s__String__charptr_long_long %92, ptr %7, align 8
  %93 = call ptr @_mf__String__charptr__getRaw(ptr %7)
  %94 = call i32 (ptr, ...) @printf(ptr @5, ptr %93)
  %95 = load %_s__String__charptr_long_long, ptr %7, align 8
  call void @_mp__String__void__ctor__String_string(ptr %8, %_s__String__charptr_long_long %95, ptr @7)
  %96 = load %_s__String__charptr_long_long, ptr %8, align 8
  store %_s__String__charptr_long_long %96, ptr %9, align 8
  %97 = call ptr @_mf__String__charptr__getRaw(ptr %9)
  %98 = call i32 (ptr, ...) @printf(ptr @6, ptr %97)
  store ptr @9, ptr %11, align 8
  %99 = load ptr, ptr %11, align 8
  call void @_mp__String__void__ctor__string(ptr %10, ptr %99)
  %100 = load %_s__String__charptr_long_long, ptr %10, align 8
  %101 = load %_s__String__charptr_long_long, ptr %7, align 8
  call void @_mp__String__void__ctor__String_String(ptr %12, %_s__String__charptr_long_long %100, %_s__String__charptr_long_long %101)
  %102 = load %_s__String__charptr_long_long, ptr %12, align 8
  store %_s__String__charptr_long_long %102, ptr %13, align 8
  %103 = call ptr @_mf__String__charptr__getRaw(ptr %13)
  %104 = call i32 (ptr, ...) @printf(ptr @8, ptr %103)
  %105 = load %_s__String__charptr_long_long, ptr %7, align 8
  %106 = load %_s__String__charptr_long_long, ptr %7, align 8
  call void @_mp__String__void__ctor__String_String(ptr %14, %_s__String__charptr_long_long %105, %_s__String__charptr_long_long %106)
  %107 = load %_s__String__charptr_long_long, ptr %14, align 8
  store %_s__String__charptr_long_long %107, ptr %15, align 8
  %108 = call ptr @_mf__String__charptr__getRaw(ptr %15)
  %109 = call i32 (ptr, ...) @printf(ptr @10, ptr %108)
  %110 = load %_s__String__charptr_long_long, ptr %7, align 8
  call void @_mp__String__void__ctor__String_string(ptr %16, %_s__String__charptr_long_long %110, ptr @12)
  %111 = load %_s__String__charptr_long_long, ptr %16, align 8
  %112 = load %_s__String__charptr_long_long, ptr %7, align 8
  call void @_mp__String__void__ctor__String_String(ptr %17, %_s__String__charptr_long_long %111, %_s__String__charptr_long_long %112)
  %113 = load %_s__String__charptr_long_long, ptr %17, align 8
  store %_s__String__charptr_long_long %113, ptr %18, align 8
  %114 = call ptr @_mf__String__charptr__getRaw(ptr %18)
  %115 = call i32 (ptr, ...) @printf(ptr @11, ptr %114)
  store ptr @14, ptr %20, align 8
  %116 = load ptr, ptr %20, align 8
  call void @_mp__String__void__ctor__string(ptr %19, ptr %116)
  %117 = load %_s__String__charptr_long_long, ptr %19, align 8
  %118 = load %_s__String__charptr_long_long, ptr %7, align 8
  call void @_mp__String__void__ctor__String_String(ptr %21, %_s__String__charptr_long_long %117, %_s__String__charptr_long_long %118)
  %119 = load %_s__String__charptr_long_long, ptr %21, align 8
  call void @_mp__String__void__ctor__String_string(ptr %22, %_s__String__charptr_long_long %119, ptr @15)
  %120 = load %_s__String__charptr_long_long, ptr %22, align 8
  store %_s__String__charptr_long_long %120, ptr %23, align 8
  %121 = call ptr @_mf__String__charptr__getRaw(ptr %23)
  %122 = call i32 (ptr, ...) @printf(ptr @13, ptr %121)
  store ptr @17, ptr %25, align 8
  %123 = load ptr, ptr %25, align 8
  call void @_mp__String__void__ctor__string(ptr %24, ptr %123)
  %124 = load %_s__String__charptr_long_long, ptr %24, align 8
  store i16 4, ptr %26, align 2
  store ptr @18, ptr %28, align 8
  %125 = load ptr, ptr %28, align 8
  call void @_mp__String__void__ctor__string(ptr %27, ptr %125)
  call void @_mp__String__void__ctor__String(ptr %29, %_s__String__charptr_long_long %124)
  call void @_mp__String__void__opMul__short(ptr %29, i16 4)
  %126 = load %_s__String__charptr_long_long, ptr %29, align 8
  store %_s__String__charptr_long_long %126, ptr %29, align 8
  %127 = call ptr @_mf__String__charptr__getRaw(ptr %29)
  %128 = call i32 (ptr, ...) @printf(ptr @16, ptr %127)
  store ptr @19, ptr %s24, align 8
  %129 = load ptr, ptr %s24, align 8
  call void @_mp__String__void__ctor__string(ptr %s2, ptr %129)
  %130 = load %_s__String__charptr_long_long, ptr %s2, align 8
  store ptr @20, ptr %s26, align 8
  %131 = load ptr, ptr %s26, align 8
  call void @_mp__String__void__ctor__string(ptr %s25, ptr %131)
  store i32 5, ptr %s27, align 4
  call void @_mp__String__void__ctor__String(ptr %30, %_s__String__charptr_long_long %130)
  call void @_mp__String__void__opMul__int(ptr %30, i32 5)
  %132 = load %_s__String__charptr_long_long, ptr %30, align 8
  store %_s__String__charptr_long_long %132, ptr %30, align 8
  %133 = call ptr @_mf__String__charptr__getRaw(ptr %30)
  %134 = call i32 (ptr, ...) @printf(ptr @21, ptr %133)
  store i8 97, ptr %32, align 1
  %135 = load i8, ptr %32, align 1
  call void @_mp__String__void__ctor__char(ptr %31, i8 %135)
  %136 = load %_s__String__charptr_long_long, ptr %31, align 8
  store i32 20, ptr %33, align 4
  store i8 97, ptr %35, align 1
  %137 = load i8, ptr %35, align 1
  call void @_mp__String__void__ctor__char(ptr %34, i8 %137)
  call void @_mp__String__void__ctor__String(ptr %36, %_s__String__charptr_long_long %136)
  call void @_mp__String__void__opMul__int(ptr %36, i32 20)
  %138 = load %_s__String__charptr_long_long, ptr %36, align 8
  store %_s__String__charptr_long_long %138, ptr %36, align 8
  %139 = call ptr @_mf__String__charptr__getRaw(ptr %36)
  %140 = call i32 (ptr, ...) @printf(ptr @22, ptr %139)
  store i8 99, ptr %s38, align 1
  %141 = load i8, ptr %s38, align 1
  call void @_mp__String__void__ctor__char(ptr %s3, i8 %141)
  %142 = load %_s__String__charptr_long_long, ptr %s3, align 8
  store i32 2, ptr %s39, align 4
  store i8 99, ptr %s311, align 1
  %143 = load i8, ptr %s311, align 1
  call void @_mp__String__void__ctor__char(ptr %s310, i8 %143)
  call void @_mp__String__void__ctor__String(ptr %37, %_s__String__charptr_long_long %142)
  call void @_mp__String__void__opMul__int(ptr %37, i32 2)
  %144 = load %_s__String__charptr_long_long, ptr %37, align 8
  store i32 7, ptr %s312, align 4
  call void @_mp__String__void__ctor__String(ptr %38, %_s__String__charptr_long_long %144)
  call void @_mp__String__void__opMul__int(ptr %38, i32 7)
  %145 = load %_s__String__charptr_long_long, ptr %38, align 8
  store %_s__String__charptr_long_long %145, ptr %38, align 8
  %146 = call ptr @_mf__String__charptr__getRaw(ptr %38)
  %147 = call i32 (ptr, ...) @printf(ptr @23, ptr %146)
  store ptr @25, ptr %39, align 8
  %148 = load ptr, ptr %39, align 8
  %149 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %148, ptr @26)
  store i1 %149, ptr %40, align 1
  %150 = load i1, ptr %40, align 1
  %151 = zext i1 %150 to i32
  %152 = call i32 (ptr, ...) @printf(ptr @24, i32 %151)
  store ptr @28, ptr %41, align 8
  %153 = load ptr, ptr %41, align 8
  %154 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %153, ptr @29)
  store i1 %154, ptr %42, align 1
  %155 = load i1, ptr %42, align 1
  %156 = zext i1 %155 to i32
  %157 = call i32 (ptr, ...) @printf(ptr @27, i32 %156)
  store ptr @31, ptr %43, align 8
  %158 = load ptr, ptr %43, align 8
  %159 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %158, ptr @32)
  store i1 %159, ptr %44, align 1
  %160 = load i1, ptr %44, align 1
  %161 = zext i1 %160 to i32
  %162 = call i32 (ptr, ...) @printf(ptr @30, i32 %161)
  store ptr @34, ptr %46, align 8
  %163 = load ptr, ptr %46, align 8
  call void @_mp__String__void__ctor__string(ptr %45, ptr %163)
  %164 = load %_s__String__charptr_long_long, ptr %45, align 8
  store ptr @35, ptr %48, align 8
  %165 = load ptr, ptr %48, align 8
  call void @_mp__String__void__ctor__string(ptr %47, ptr %165)
  %166 = load %_s__String__charptr_long_long, ptr %47, align 8
  %167 = call i1 @_mf__String__bool__isEqual__String(ptr %45, %_s__String__charptr_long_long %166)
  store i1 %167, ptr %49, align 1
  %168 = load i1, ptr %49, align 1
  %169 = zext i1 %168 to i32
  %170 = call i32 (ptr, ...) @printf(ptr @33, i32 %169)
  store ptr @37, ptr %51, align 8
  %171 = load ptr, ptr %51, align 8
  call void @_mp__String__void__ctor__string(ptr %50, ptr %171)
  %172 = load %_s__String__charptr_long_long, ptr %50, align 8
  store ptr @38, ptr %53, align 8
  %173 = load ptr, ptr %53, align 8
  call void @_mp__String__void__ctor__string(ptr %52, ptr %173)
  %174 = load %_s__String__charptr_long_long, ptr %52, align 8
  %175 = call i1 @_mf__String__bool__isEqual__String(ptr %50, %_s__String__charptr_long_long %174)
  store i1 %175, ptr %54, align 1
  %176 = load i1, ptr %54, align 1
  %177 = zext i1 %176 to i32
  %178 = call i32 (ptr, ...) @printf(ptr @36, i32 %177)
  store ptr @40, ptr %56, align 8
  %179 = load ptr, ptr %56, align 8
  call void @_mp__String__void__ctor__string(ptr %55, ptr %179)
  %180 = load %_s__String__charptr_long_long, ptr %55, align 8
  store ptr @41, ptr %58, align 8
  %181 = load ptr, ptr %58, align 8
  call void @_mp__String__void__ctor__string(ptr %57, ptr %181)
  %182 = load %_s__String__charptr_long_long, ptr %57, align 8
  %183 = call i1 @_mf__String__bool__isEqual__String(ptr %55, %_s__String__charptr_long_long %182)
  store i1 %183, ptr %59, align 1
  %184 = load i1, ptr %59, align 1
  %185 = zext i1 %184 to i32
  %186 = call i32 (ptr, ...) @printf(ptr @39, i32 %185)
  store ptr @43, ptr %60, align 8
  %187 = load ptr, ptr %60, align 8
  %188 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %187, ptr @44)
  %189 = xor i1 %188, true
  store i1 %189, ptr %61, align 1
  %190 = load i1, ptr %61, align 1
  %191 = zext i1 %190 to i32
  %192 = call i32 (ptr, ...) @printf(ptr @42, i32 %191)
  store ptr @46, ptr %62, align 8
  %193 = load ptr, ptr %62, align 8
  %194 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %193, ptr @47)
  %195 = xor i1 %194, true
  store i1 %195, ptr %63, align 1
  %196 = load i1, ptr %63, align 1
  %197 = zext i1 %196 to i32
  %198 = call i32 (ptr, ...) @printf(ptr @45, i32 %197)
  store ptr @49, ptr %64, align 8
  %199 = load ptr, ptr %64, align 8
  %200 = call i1 @_f__void__bool__isRawEqual__string_string(ptr %199, ptr @50)
  %201 = xor i1 %200, true
  store i1 %201, ptr %65, align 1
  %202 = load i1, ptr %65, align 1
  %203 = zext i1 %202 to i32
  %204 = call i32 (ptr, ...) @printf(ptr @48, i32 %203)
  store ptr @52, ptr %67, align 8
  %205 = load ptr, ptr %67, align 8
  call void @_mp__String__void__ctor__string(ptr %66, ptr %205)
  %206 = load %_s__String__charptr_long_long, ptr %66, align 8
  store ptr @53, ptr %69, align 8
  %207 = load ptr, ptr %69, align 8
  call void @_mp__String__void__ctor__string(ptr %68, ptr %207)
  %208 = load %_s__String__charptr_long_long, ptr %68, align 8
  %209 = call i1 @_mf__String__bool__isEqual__String(ptr %66, %_s__String__charptr_long_long %208)
  %210 = xor i1 %209, true
  store i1 %210, ptr %70, align 1
  %211 = load i1, ptr %70, align 1
  %212 = zext i1 %211 to i32
  %213 = call i32 (ptr, ...) @printf(ptr @51, i32 %212)
  store ptr @55, ptr %72, align 8
  %214 = load ptr, ptr %72, align 8
  call void @_mp__String__void__ctor__string(ptr %71, ptr %214)
  %215 = load %_s__String__charptr_long_long, ptr %71, align 8
  store ptr @56, ptr %74, align 8
  %216 = load ptr, ptr %74, align 8
  call void @_mp__String__void__ctor__string(ptr %73, ptr %216)
  %217 = load %_s__String__charptr_long_long, ptr %73, align 8
  %218 = call i1 @_mf__String__bool__isEqual__String(ptr %71, %_s__String__charptr_long_long %217)
  %219 = xor i1 %218, true
  store i1 %219, ptr %75, align 1
  %220 = load i1, ptr %75, align 1
  %221 = zext i1 %220 to i32
  %222 = call i32 (ptr, ...) @printf(ptr @54, i32 %221)
  store ptr @58, ptr %77, align 8
  %223 = load ptr, ptr %77, align 8
  call void @_mp__String__void__ctor__string(ptr %76, ptr %223)
  %224 = load %_s__String__charptr_long_long, ptr %76, align 8
  store ptr @59, ptr %79, align 8
  %225 = load ptr, ptr %79, align 8
  call void @_mp__String__void__ctor__string(ptr %78, ptr %225)
  %226 = load %_s__String__charptr_long_long, ptr %78, align 8
  %227 = call i1 @_mf__String__bool__isEqual__String(ptr %76, %_s__String__charptr_long_long %226)
  %228 = xor i1 %227, true
  store i1 %228, ptr %80, align 1
  %229 = load i1, ptr %80, align 1
  %230 = zext i1 %229 to i32
  %231 = call i32 (ptr, ...) @printf(ptr @57, i32 %230)
  store ptr @60, ptr %s413, align 8
  %232 = load ptr, ptr %s413, align 8
  call void @_mp__String__void__ctor__string(ptr %s4, ptr %232)
  %233 = load %_s__String__charptr_long_long, ptr %s4, align 8
  call void @_mp__String__void__append__char(ptr %s4, i8 108)
  %234 = call ptr @_mf__String__charptr__getRaw(ptr %s4)
  %235 = call i32 (ptr, ...) @printf(ptr @61, ptr %234)
  store ptr @62, ptr %s514, align 8
  %236 = load ptr, ptr %s514, align 8
  call void @_mp__String__void__ctor__string(ptr %s5, ptr %236)
  %237 = load %_s__String__charptr_long_long, ptr %s5, align 8
  call void @_mp__String__void__append__string(ptr %s5, ptr @63)
  %238 = call ptr @_mf__String__charptr__getRaw(ptr %s5)
  %239 = call i32 (ptr, ...) @printf(ptr @64, ptr %238)
  store ptr @65, ptr %s615, align 8
  %240 = load ptr, ptr %s615, align 8
  call void @_mp__String__void__ctor__string(ptr %s6, ptr %240)
  %241 = load %_s__String__charptr_long_long, ptr %s6, align 8
  call void @_mp__String__void__opMul__int(ptr %s6, i32 3)
  %242 = call ptr @_mf__String__charptr__getRaw(ptr %s6)
  %243 = call i32 (ptr, ...) @printf(ptr @66, ptr %242)
  br label %cleanup.l1

cleanup.l1:                                       ; preds = %entry.l1
  call void @_mp__String__void__dtor(ptr %19)
  call void @_mp__String__void__dtor(ptr %29)
  call void @_mp__String__void__dtor(ptr %s25)
  call void @_mp__String__void__dtor(ptr %36)
  call void @_mp__String__void__dtor(ptr %37)
  call void @_mp__String__void__dtor(ptr %45)
  call void @_mp__String__void__dtor(ptr %47)
  call void @_mp__String__void__dtor(ptr %50)
  call void @_mp__String__void__dtor(ptr %52)
  call void @_mp__String__void__dtor(ptr %55)
  call void @_mp__String__void__dtor(ptr %57)
  call void @_mp__String__void__dtor(ptr %66)
  call void @_mp__String__void__dtor(ptr %68)
  call void @_mp__String__void__dtor(ptr %71)
  call void @_mp__String__void__dtor(ptr %73)
  call void @_mp__String__void__dtor(ptr %76)
  call void @_mp__String__void__dtor(ptr %78)
  call void @_mp__String__void__dtor(ptr %0)
  call void @_mp__String__void__dtor(ptr %4)
  call void @_mp__String__void__dtor(ptr %s4)
  call void @_mp__String__void__dtor(ptr %s5)
  call void @_mp__String__void__dtor(ptr %s6)
  call void @_mp__String__void__dtor(ptr %s1)
  call void @_mp__String__void__dtor(ptr %6)
  call void @_mp__String__void__dtor(ptr %10)
  %244 = load i32, ptr %result, align 4
  ret i32 %244
}

declare i32 @printf(ptr, ...)

declare void @_mp__String__void__ctor__string(ptr, ptr)

declare void @_mp__String__void__ctor__String_String(ptr, %_s__String__charptr_long_long, %_s__String__charptr_long_long)

declare ptr @_mf__String__charptr__getRaw(ptr)

declare void @_mp__String__void__ctor__String_string(ptr, %_s__String__charptr_long_long, ptr)

declare void @_mp__String__void__ctor__String(ptr, %_s__String__charptr_long_long)

declare void @_mp__String__void__opMul__short(ptr, i16)

declare void @_mp__String__void__opMul__int(ptr, i32)

declare void @_mp__String__void__ctor__char(ptr, i8)

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr)

declare i1 @_mf__String__bool__isEqual__String(ptr, %_s__String__charptr_long_long)

declare void @_mp__String__void__append__char(ptr, i8)

declare void @_mp__String__void__append__string(ptr, ptr)

declare void @_mp__String__void__dtor(ptr)
