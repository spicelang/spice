; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.3 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.4 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@printf.str.3 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.5 = private unnamed_addr constant [5 x i8] c"Hi! \00", align 1
@printf.str.4 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@printf.str.6 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.7 = private unnamed_addr constant [8 x i8] c"Prefix \00", align 1
@anon.string.8 = private unnamed_addr constant [8 x i8] c" Suffix\00", align 1
@printf.str.7 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.9 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@anon.string.10 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@printf.str.8 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.9 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.10 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.11 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.11 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.12 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.12 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.13 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.14 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.13 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.15 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.16 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.14 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.17 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.18 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.15 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.19 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.20 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.16 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.21 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.22 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.17 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.23 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.24 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.18 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.25 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.26 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.19 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.27 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.28 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.20 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.29 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.30 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.21 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.31 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.32 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.22 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.33 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.34 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %s1 = alloca %__String__charptr_long_long, align 8
  %5 = alloca %__String__charptr_long_long, align 8
  %6 = alloca %__String__charptr_long_long, align 8
  %7 = alloca %__String__charptr_long_long, align 8
  %8 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %9 = alloca %__String__charptr_long_long, align 8
  %10 = alloca %__String__charptr_long_long, align 8
  %s3 = alloca %__String__charptr_long_long, align 8
  %11 = alloca %__String__charptr_long_long, align 8
  %12 = alloca %__String__charptr_long_long, align 8
  %13 = alloca %__String__charptr_long_long, align 8
  %14 = alloca %__String__charptr_long_long, align 8
  %15 = alloca %__String__charptr_long_long, align 8
  %16 = alloca %__String__charptr_long_long, align 8
  %17 = alloca %__String__charptr_long_long, align 8
  %18 = alloca %__String__charptr_long_long, align 8
  %19 = alloca %__String__charptr_long_long, align 8
  %20 = alloca %__String__charptr_long_long, align 8
  %21 = alloca %__String__charptr_long_long, align 8
  %22 = alloca %__String__charptr_long_long, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__String__void__ctor__string(ptr %1, ptr @anon.string.0)
  call void @_mp__String__void__ctor__string(ptr %2, ptr @anon.string.1)
  %23 = load %__String__charptr_long_long, ptr %1, align 8
  %24 = load %__String__charptr_long_long, ptr %2, align 8
  %25 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %23, %__String__charptr_long_long %24)
  %26 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, %__String__charptr_long_long %25)
  call void @_mp__String__void__ctor__string(ptr %3, ptr @anon.string.2)
  call void @_mp__String__void__ctor__string(ptr %4, ptr @anon.string.3)
  %27 = load %__String__charptr_long_long, ptr %3, align 8
  %28 = load %__String__charptr_long_long, ptr %4, align 8
  %29 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %27, %__String__charptr_long_long %28)
  store %__String__charptr_long_long %29, ptr %s1, align 8
  %30 = load %__String__charptr_long_long, ptr %s1, align 8
  %31 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, %__String__charptr_long_long %30)
  %32 = load %__String__charptr_long_long, ptr %s1, align 8
  %33 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %32, ptr @anon.string.4)
  %34 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, %__String__charptr_long_long %33)
  call void @_mp__String__void__ctor__string(ptr %5, ptr @anon.string.5)
  %35 = load %__String__charptr_long_long, ptr %5, align 8
  %36 = load %__String__charptr_long_long, ptr %s1, align 8
  %37 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %35, %__String__charptr_long_long %36)
  %38 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, %__String__charptr_long_long %37)
  %39 = load %__String__charptr_long_long, ptr %s1, align 8
  %40 = load %__String__charptr_long_long, ptr %s1, align 8
  %41 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %39, %__String__charptr_long_long %40)
  %42 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, %__String__charptr_long_long %41)
  %43 = load %__String__charptr_long_long, ptr %s1, align 8
  %44 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %43, ptr @anon.string.6)
  %45 = load %__String__charptr_long_long, ptr %s1, align 8
  %46 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %44, %__String__charptr_long_long %45)
  %47 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, %__String__charptr_long_long %46)
  call void @_mp__String__void__ctor__string(ptr %6, ptr @anon.string.7)
  %48 = load %__String__charptr_long_long, ptr %6, align 8
  %49 = load %__String__charptr_long_long, ptr %s1, align 8
  %50 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %48, %__String__charptr_long_long %49)
  %51 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %50, ptr @anon.string.8)
  %52 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, %__String__charptr_long_long %51)
  call void @_mp__String__void__ctor__string(ptr %7, ptr @anon.string.9)
  %53 = load %__String__charptr_long_long, ptr %7, align 8
  %54 = call %__String__charptr_long_long @_f__void__String__op.mul__short_String(i16 4, %__String__charptr_long_long %53)
  %55 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, %__String__charptr_long_long %54)
  call void @_mp__String__void__ctor__string(ptr %8, ptr @anon.string.10)
  %56 = load %__String__charptr_long_long, ptr %8, align 8
  %57 = call %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long %56, i32 5)
  store %__String__charptr_long_long %57, ptr %s2, align 8
  %58 = load %__String__charptr_long_long, ptr %s2, align 8
  %59 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, %__String__charptr_long_long %58)
  call void @_mp__String__void__ctor__char(ptr %9, i8 97)
  %60 = load %__String__charptr_long_long, ptr %9, align 8
  %61 = call %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32 20, %__String__charptr_long_long %60)
  %62 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.9, %__String__charptr_long_long %61)
  call void @_mp__String__void__ctor__char(ptr %10, i8 99)
  %63 = load %__String__charptr_long_long, ptr %10, align 8
  %64 = call %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32 2, %__String__charptr_long_long %63)
  %65 = call %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long %64, i32 7)
  store %__String__charptr_long_long %65, ptr %s3, align 8
  %66 = load %__String__charptr_long_long, ptr %s3, align 8
  %67 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.10, %__String__charptr_long_long %66)
  %68 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.11, ptr @anon.string.12)
  %69 = zext i1 %68 to i32
  %70 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.11, i32 %69)
  %71 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.13, ptr @anon.string.14)
  %72 = zext i1 %71 to i32
  %73 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.12, i32 %72)
  %74 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.15, ptr @anon.string.16)
  %75 = zext i1 %74 to i32
  %76 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.13, i32 %75)
  call void @_mp__String__void__ctor__string(ptr %11, ptr @anon.string.17)
  call void @_mp__String__void__ctor__string(ptr %12, ptr @anon.string.18)
  %77 = load %__String__charptr_long_long, ptr %11, align 8
  %78 = load %__String__charptr_long_long, ptr %12, align 8
  %79 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %77, %__String__charptr_long_long %78)
  %80 = zext i1 %79 to i32
  %81 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.14, i32 %80)
  call void @_mp__String__void__ctor__string(ptr %13, ptr @anon.string.19)
  call void @_mp__String__void__ctor__string(ptr %14, ptr @anon.string.20)
  %82 = load %__String__charptr_long_long, ptr %13, align 8
  %83 = load %__String__charptr_long_long, ptr %14, align 8
  %84 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %82, %__String__charptr_long_long %83)
  %85 = zext i1 %84 to i32
  %86 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.15, i32 %85)
  call void @_mp__String__void__ctor__string(ptr %15, ptr @anon.string.21)
  call void @_mp__String__void__ctor__string(ptr %16, ptr @anon.string.22)
  %87 = load %__String__charptr_long_long, ptr %15, align 8
  %88 = load %__String__charptr_long_long, ptr %16, align 8
  %89 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %87, %__String__charptr_long_long %88)
  %90 = zext i1 %89 to i32
  %91 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.16, i32 %90)
  %92 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.23, ptr @anon.string.24)
  %93 = xor i1 %92, true
  %94 = zext i1 %93 to i32
  %95 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.17, i32 %94)
  %96 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.25, ptr @anon.string.26)
  %97 = xor i1 %96, true
  %98 = zext i1 %97 to i32
  %99 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.18, i32 %98)
  %100 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.27, ptr @anon.string.28)
  %101 = xor i1 %100, true
  %102 = zext i1 %101 to i32
  %103 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.19, i32 %102)
  call void @_mp__String__void__ctor__string(ptr %17, ptr @anon.string.29)
  call void @_mp__String__void__ctor__string(ptr %18, ptr @anon.string.30)
  %104 = load %__String__charptr_long_long, ptr %17, align 8
  %105 = load %__String__charptr_long_long, ptr %18, align 8
  %106 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %104, %__String__charptr_long_long %105)
  %107 = zext i1 %106 to i32
  %108 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.20, i32 %107)
  call void @_mp__String__void__ctor__string(ptr %19, ptr @anon.string.31)
  call void @_mp__String__void__ctor__string(ptr %20, ptr @anon.string.32)
  %109 = load %__String__charptr_long_long, ptr %19, align 8
  %110 = load %__String__charptr_long_long, ptr %20, align 8
  %111 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %109, %__String__charptr_long_long %110)
  %112 = zext i1 %111 to i32
  %113 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.21, i32 %112)
  call void @_mp__String__void__ctor__string(ptr %21, ptr @anon.string.33)
  call void @_mp__String__void__ctor__string(ptr %22, ptr @anon.string.34)
  %114 = load %__String__charptr_long_long, ptr %21, align 8
  %115 = load %__String__charptr_long_long, ptr %22, align 8
  %116 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %114, %__String__charptr_long_long %115)
  %117 = zext i1 %116 to i32
  %118 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.22, i32 %117)
  %119 = load i32, ptr %result, align 4
  ret i32 %119
}

declare i32 @printf(ptr noundef, ...)

declare void @_mp__String__void__ctor__string(ptr, ptr)

declare %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long, %__String__charptr_long_long)

declare %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long, ptr)

declare %__String__charptr_long_long @_f__void__String__op.mul__short_String(i16, %__String__charptr_long_long)

declare %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long, i32)

declare void @_mp__String__void__ctor__char(ptr, i8)

declare %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32, %__String__charptr_long_long)

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr)

declare i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long, %__String__charptr_long_long)

declare i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long, %__String__charptr_long_long)

attributes #0 = { noinline nounwind optnone uwtable }
