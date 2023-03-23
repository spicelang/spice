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
@anon.string.35 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.23 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.36 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@anon.string.37 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@printf.str.24 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.38 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@printf.str.25 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %s1 = alloca %__String__charptr_long_long, align 8
  %5 = alloca ptr, align 8
  %6 = alloca %__String__charptr_long_long, align 8
  %7 = alloca ptr, align 8
  %8 = alloca %__String__charptr_long_long, align 8
  %9 = alloca %__String__charptr_long_long, align 8
  %10 = alloca %__String__charptr_long_long, align 8
  %11 = alloca ptr, align 8
  %12 = alloca %__String__charptr_long_long, align 8
  %13 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %14 = alloca %__String__charptr_long_long, align 8
  %15 = alloca %__String__charptr_long_long, align 8
  %16 = alloca %__String__charptr_long_long, align 8
  %s3 = alloca %__String__charptr_long_long, align 8
  %17 = alloca %__String__charptr_long_long, align 8
  %18 = alloca %__String__charptr_long_long, align 8
  %19 = alloca %__String__charptr_long_long, align 8
  %20 = alloca %__String__charptr_long_long, align 8
  %21 = alloca %__String__charptr_long_long, align 8
  %22 = alloca %__String__charptr_long_long, align 8
  %23 = alloca %__String__charptr_long_long, align 8
  %24 = alloca %__String__charptr_long_long, align 8
  %25 = alloca %__String__charptr_long_long, align 8
  %26 = alloca %__String__charptr_long_long, align 8
  %27 = alloca %__String__charptr_long_long, align 8
  %28 = alloca %__String__charptr_long_long, align 8
  %s4 = alloca %__String__charptr_long_long, align 8
  %29 = alloca i8, align 1
  %s5 = alloca %__String__charptr_long_long, align 8
  %30 = alloca ptr, align 8
  %s6 = alloca %__String__charptr_long_long, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__String__void__ctor__string(ptr %1, ptr @anon.string.0)
  call void @_mp__String__void__ctor__string(ptr %2, ptr @anon.string.1)
  %31 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %1, ptr %2)
  %32 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, %__String__charptr_long_long %31)
  call void @_mp__String__void__ctor__string(ptr %3, ptr @anon.string.2)
  call void @_mp__String__void__ctor__string(ptr %4, ptr @anon.string.3)
  %33 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %3, ptr %4)
  store %__String__charptr_long_long %33, ptr %s1, align 8
  store %__String__charptr_long_long %33, ptr %s1, align 8
  %34 = load %__String__charptr_long_long, ptr %s1, align 8
  %35 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, %__String__charptr_long_long %34)
  store ptr @anon.string.4, ptr %5, align 8
  %36 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr %s1, ptr %5)
  %37 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, %__String__charptr_long_long %36)
  call void @_mp__String__void__ctor__string(ptr %6, ptr @anon.string.5)
  %38 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %6, ptr %s1)
  %39 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, %__String__charptr_long_long %38)
  %40 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %s1, ptr %s1)
  %41 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, %__String__charptr_long_long %40)
  store ptr @anon.string.6, ptr %7, align 8
  %42 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr %s1, ptr %7)
  store %__String__charptr_long_long %42, ptr %8, align 8
  %43 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %8, ptr %s1)
  %44 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, %__String__charptr_long_long %43)
  call void @_mp__String__void__ctor__string(ptr %9, ptr @anon.string.7)
  %45 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %9, ptr %s1)
  store %__String__charptr_long_long %45, ptr %10, align 8
  store ptr @anon.string.8, ptr %11, align 8
  %46 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr %10, ptr %11)
  %47 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, %__String__charptr_long_long %46)
  call void @_mp__String__void__ctor__string(ptr %12, ptr @anon.string.9)
  %48 = call %__String__charptr_long_long @_f__void__String__op.mul__short_Stringref(i16 4, ptr %12)
  %49 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, %__String__charptr_long_long %48)
  call void @_mp__String__void__ctor__string(ptr %13, ptr @anon.string.10)
  %50 = call %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr %13, i32 5)
  store %__String__charptr_long_long %50, ptr %s2, align 8
  store %__String__charptr_long_long %50, ptr %s2, align 8
  %51 = load %__String__charptr_long_long, ptr %s2, align 8
  %52 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, %__String__charptr_long_long %51)
  call void @_mp__String__void__ctor__char(ptr %14, i8 97)
  %53 = call %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32 20, ptr %14)
  %54 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.9, %__String__charptr_long_long %53)
  call void @_mp__String__void__ctor__char(ptr %15, i8 99)
  %55 = call %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32 2, ptr %15)
  store %__String__charptr_long_long %55, ptr %16, align 8
  %56 = call %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr %16, i32 7)
  store %__String__charptr_long_long %56, ptr %s3, align 8
  store %__String__charptr_long_long %56, ptr %s3, align 8
  %57 = load %__String__charptr_long_long, ptr %s3, align 8
  %58 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.10, %__String__charptr_long_long %57)
  %59 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.11, ptr @anon.string.12)
  %60 = zext i1 %59 to i32
  %61 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.11, i32 %60)
  %62 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.13, ptr @anon.string.14)
  %63 = zext i1 %62 to i32
  %64 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.12, i32 %63)
  %65 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.15, ptr @anon.string.16)
  %66 = zext i1 %65 to i32
  %67 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.13, i32 %66)
  call void @_mp__String__void__ctor__string(ptr %17, ptr @anon.string.17)
  call void @_mp__String__void__ctor__string(ptr %18, ptr @anon.string.18)
  %68 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr %17, ptr %18)
  %69 = zext i1 %68 to i32
  %70 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.14, i32 %69)
  call void @_mp__String__void__ctor__string(ptr %19, ptr @anon.string.19)
  call void @_mp__String__void__ctor__string(ptr %20, ptr @anon.string.20)
  %71 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr %19, ptr %20)
  %72 = zext i1 %71 to i32
  %73 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.15, i32 %72)
  call void @_mp__String__void__ctor__string(ptr %21, ptr @anon.string.21)
  call void @_mp__String__void__ctor__string(ptr %22, ptr @anon.string.22)
  %74 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr %21, ptr %22)
  %75 = zext i1 %74 to i32
  %76 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.16, i32 %75)
  %77 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.23, ptr @anon.string.24)
  %78 = xor i1 %77, true
  %79 = zext i1 %78 to i32
  %80 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.17, i32 %79)
  %81 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.25, ptr @anon.string.26)
  %82 = xor i1 %81, true
  %83 = zext i1 %82 to i32
  %84 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.18, i32 %83)
  %85 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.27, ptr @anon.string.28)
  %86 = xor i1 %85, true
  %87 = zext i1 %86 to i32
  %88 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.19, i32 %87)
  call void @_mp__String__void__ctor__string(ptr %23, ptr @anon.string.29)
  call void @_mp__String__void__ctor__string(ptr %24, ptr @anon.string.30)
  %89 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr %23, ptr %24)
  %90 = zext i1 %89 to i32
  %91 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.20, i32 %90)
  call void @_mp__String__void__ctor__string(ptr %25, ptr @anon.string.31)
  call void @_mp__String__void__ctor__string(ptr %26, ptr @anon.string.32)
  %92 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr %25, ptr %26)
  %93 = zext i1 %92 to i32
  %94 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.21, i32 %93)
  call void @_mp__String__void__ctor__string(ptr %27, ptr @anon.string.33)
  call void @_mp__String__void__ctor__string(ptr %28, ptr @anon.string.34)
  %95 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr %27, ptr %28)
  %96 = zext i1 %95 to i32
  %97 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.22, i32 %96)
  call void @_mp__String__void__ctor__string(ptr %s4, ptr @anon.string.35)
  store i8 108, ptr %29, align 1
  call void @_p__void__void__op.plusequal__Stringref_charref(ptr %s4, ptr %29)
  %98 = load %__String__charptr_long_long, ptr %s4, align 8
  %99 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.23, %__String__charptr_long_long %98)
  call void @_mp__String__void__ctor__string(ptr %s5, ptr @anon.string.36)
  store ptr @anon.string.37, ptr %30, align 8
  call void @_p__void__void__op.plusequal__Stringref_stringref(ptr %s5, ptr %30)
  %100 = load %__String__charptr_long_long, ptr %s5, align 8
  %101 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.24, %__String__charptr_long_long %100)
  call void @_mp__String__void__ctor__string(ptr %s6, ptr @anon.string.38)
  call void @_p__void__void__op.mulequal__Stringref_int(ptr %s6, i32 3)
  %102 = load %__String__charptr_long_long, ptr %s6, align 8
  %103 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.25, %__String__charptr_long_long %102)
  %104 = load i32, ptr %result, align 4
  ret i32 %104
}

declare i32 @printf(ptr noundef, ...)

declare void @_mp__String__void__ctor__string(ptr, ptr)

declare %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr, ptr)

declare %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr, ptr)

declare %__String__charptr_long_long @_f__void__String__op.mul__short_Stringref(i16, ptr)

declare %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr, i32)

declare void @_mp__String__void__ctor__char(ptr, i8)

declare %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32, ptr)

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr)

declare i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr, ptr)

declare i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr, ptr)

declare void @_p__void__void__op.plusequal__Stringref_charref(ptr, ptr)

declare void @_p__void__void__op.plusequal__Stringref_stringref(ptr, ptr)

declare void @_p__void__void__op.mulequal__Stringref_int(ptr, i32)

attributes #0 = { noinline nounwind optnone uwtable }
