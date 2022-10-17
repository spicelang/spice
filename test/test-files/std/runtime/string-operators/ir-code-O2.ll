; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@1 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"Hi! \00", align 1
@3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@4 = private unnamed_addr constant [8 x i8] c"Prefix \00", align 1
@5 = private unnamed_addr constant [8 x i8] c" Suffix\00", align 1
@6 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@7 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@8 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@9 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@10 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@11 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@12 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@13 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@14 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@15 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l1:
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
  %21 = alloca %_s__String__charptr_long_long, align 8
  %22 = alloca %_s__String__charptr_long_long, align 8
  %23 = alloca %_s__String__charptr_long_long, align 8
  %24 = alloca %_s__String__charptr_long_long, align 8
  %25 = alloca %_s__String__charptr_long_long, align 8
  %26 = alloca %_s__String__charptr_long_long, align 8
  %27 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__ctor__string_string(ptr nonnull %0, ptr nonnull @6, ptr nonnull @0)
  %.fca.0.load214 = load ptr, ptr %0, align 8
  %.fca.1.gep216 = getelementptr inbounds %_s__String__charptr_long_long, ptr %0, i64 0, i32 1
  %.fca.1.load217 = load i64, ptr %.fca.1.gep216, align 8
  %.fca.2.gep219 = getelementptr inbounds %_s__String__charptr_long_long, ptr %0, i64 0, i32 2
  %.fca.2.load220 = load i64, ptr %.fca.2.gep219, align 8
  store ptr %.fca.0.load214, ptr %1, align 8
  %.fca.1.gep210 = getelementptr inbounds %_s__String__charptr_long_long, ptr %1, i64 0, i32 1
  store i64 %.fca.1.load217, ptr %.fca.1.gep210, align 8
  %.fca.2.gep212 = getelementptr inbounds %_s__String__charptr_long_long, ptr %1, i64 0, i32 2
  store i64 %.fca.2.load220, ptr %.fca.2.gep212, align 8
  %28 = call ptr @_mf__String__getRaw(ptr nonnull %1)
  %29 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %28)
  call void @_mp__String__ctor__string_string(ptr nonnull %2, ptr nonnull @6, ptr nonnull @0)
  %.fca.0.load199 = load ptr, ptr %2, align 8
  %.fca.1.gep201 = getelementptr inbounds %_s__String__charptr_long_long, ptr %2, i64 0, i32 1
  %.fca.1.load202 = load i64, ptr %.fca.1.gep201, align 8
  %.fca.2.gep204 = getelementptr inbounds %_s__String__charptr_long_long, ptr %2, i64 0, i32 2
  %.fca.2.load205 = load i64, ptr %.fca.2.gep204, align 8
  store ptr %.fca.0.load199, ptr %3, align 8
  %.fca.1.gep168 = getelementptr inbounds %_s__String__charptr_long_long, ptr %3, i64 0, i32 1
  store i64 %.fca.1.load202, ptr %.fca.1.gep168, align 8
  %.fca.2.gep170 = getelementptr inbounds %_s__String__charptr_long_long, ptr %3, i64 0, i32 2
  store i64 %.fca.2.load205, ptr %.fca.2.gep170, align 8
  %30 = call ptr @_mf__String__getRaw(ptr nonnull %3)
  %31 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %30)
  %.fca.0.load172 = load ptr, ptr %3, align 8
  %.fca.1.load175 = load i64, ptr %.fca.1.gep168, align 8
  %.fca.2.load178 = load i64, ptr %.fca.2.gep170, align 8
  store ptr %.fca.0.load172, ptr %4, align 8
  %.fca.1.gep162 = getelementptr inbounds %_s__String__charptr_long_long, ptr %4, i64 0, i32 1
  store i64 %.fca.1.load175, ptr %.fca.1.gep162, align 8
  %.fca.2.gep164 = getelementptr inbounds %_s__String__charptr_long_long, ptr %4, i64 0, i32 2
  store i64 %.fca.2.load178, ptr %.fca.2.gep164, align 8
  %32 = call ptr @_mf__String__getRaw(ptr nonnull %4)
  call void @_mp__String__ctor__string_string(ptr nonnull %5, ptr %32, ptr nonnull @1)
  %.fca.0.load151 = load ptr, ptr %5, align 8
  %.fca.1.gep153 = getelementptr inbounds %_s__String__charptr_long_long, ptr %5, i64 0, i32 1
  %.fca.1.load154 = load i64, ptr %.fca.1.gep153, align 8
  %.fca.2.gep156 = getelementptr inbounds %_s__String__charptr_long_long, ptr %5, i64 0, i32 2
  %.fca.2.load157 = load i64, ptr %.fca.2.gep156, align 8
  store ptr %.fca.0.load151, ptr %6, align 8
  %.fca.1.gep147 = getelementptr inbounds %_s__String__charptr_long_long, ptr %6, i64 0, i32 1
  store i64 %.fca.1.load154, ptr %.fca.1.gep147, align 8
  %.fca.2.gep149 = getelementptr inbounds %_s__String__charptr_long_long, ptr %6, i64 0, i32 2
  store i64 %.fca.2.load157, ptr %.fca.2.gep149, align 8
  %33 = call ptr @_mf__String__getRaw(ptr nonnull %6)
  %34 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %33)
  %35 = call ptr @_mf__String__getRaw(ptr nonnull %3)
  call void @_mp__String__ctor__string_string(ptr nonnull %7, ptr nonnull @2, ptr %35)
  %.fca.0.load136 = load ptr, ptr %7, align 8
  %.fca.1.gep138 = getelementptr inbounds %_s__String__charptr_long_long, ptr %7, i64 0, i32 1
  %.fca.1.load139 = load i64, ptr %.fca.1.gep138, align 8
  %.fca.2.gep141 = getelementptr inbounds %_s__String__charptr_long_long, ptr %7, i64 0, i32 2
  %.fca.2.load142 = load i64, ptr %.fca.2.gep141, align 8
  store ptr %.fca.0.load136, ptr %8, align 8
  %.fca.1.gep132 = getelementptr inbounds %_s__String__charptr_long_long, ptr %8, i64 0, i32 1
  store i64 %.fca.1.load139, ptr %.fca.1.gep132, align 8
  %.fca.2.gep134 = getelementptr inbounds %_s__String__charptr_long_long, ptr %8, i64 0, i32 2
  store i64 %.fca.2.load142, ptr %.fca.2.gep134, align 8
  %36 = call ptr @_mf__String__getRaw(ptr nonnull %8)
  %37 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %36)
  %.fca.0.load181 = load ptr, ptr %3, align 8
  %.fca.1.load184 = load i64, ptr %.fca.1.gep168, align 8
  %.fca.2.load187 = load i64, ptr %.fca.2.gep170, align 8
  store ptr %.fca.0.load181, ptr %9, align 8
  %.fca.1.gep126 = getelementptr inbounds %_s__String__charptr_long_long, ptr %9, i64 0, i32 1
  store i64 %.fca.1.load184, ptr %.fca.1.gep126, align 8
  %.fca.2.gep128 = getelementptr inbounds %_s__String__charptr_long_long, ptr %9, i64 0, i32 2
  store i64 %.fca.2.load187, ptr %.fca.2.gep128, align 8
  %38 = call ptr @_mf__String__getRaw(ptr nonnull %9)
  %39 = call ptr @_mf__String__getRaw(ptr nonnull %3)
  call void @_mp__String__ctor__string_string(ptr nonnull %10, ptr %38, ptr %39)
  %.fca.0.load115 = load ptr, ptr %10, align 8
  %.fca.1.gep117 = getelementptr inbounds %_s__String__charptr_long_long, ptr %10, i64 0, i32 1
  %.fca.1.load118 = load i64, ptr %.fca.1.gep117, align 8
  %.fca.2.gep120 = getelementptr inbounds %_s__String__charptr_long_long, ptr %10, i64 0, i32 2
  %.fca.2.load121 = load i64, ptr %.fca.2.gep120, align 8
  store ptr %.fca.0.load115, ptr %11, align 8
  %.fca.1.gep111 = getelementptr inbounds %_s__String__charptr_long_long, ptr %11, i64 0, i32 1
  store i64 %.fca.1.load118, ptr %.fca.1.gep111, align 8
  %.fca.2.gep113 = getelementptr inbounds %_s__String__charptr_long_long, ptr %11, i64 0, i32 2
  store i64 %.fca.2.load121, ptr %.fca.2.gep113, align 8
  %40 = call ptr @_mf__String__getRaw(ptr nonnull %11)
  %41 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %40)
  %.fca.0.load190 = load ptr, ptr %3, align 8
  %.fca.1.load193 = load i64, ptr %.fca.1.gep168, align 8
  %.fca.2.load196 = load i64, ptr %.fca.2.gep170, align 8
  store ptr %.fca.0.load190, ptr %12, align 8
  %.fca.1.gep105 = getelementptr inbounds %_s__String__charptr_long_long, ptr %12, i64 0, i32 1
  store i64 %.fca.1.load193, ptr %.fca.1.gep105, align 8
  %.fca.2.gep107 = getelementptr inbounds %_s__String__charptr_long_long, ptr %12, i64 0, i32 2
  store i64 %.fca.2.load196, ptr %.fca.2.gep107, align 8
  %42 = call ptr @_mf__String__getRaw(ptr nonnull %12)
  call void @_mp__String__ctor__string_string(ptr nonnull %13, ptr %42, ptr nonnull @3)
  %.fca.0.load94 = load ptr, ptr %13, align 8
  %.fca.1.gep96 = getelementptr inbounds %_s__String__charptr_long_long, ptr %13, i64 0, i32 1
  %.fca.1.load97 = load i64, ptr %.fca.1.gep96, align 8
  %.fca.2.gep99 = getelementptr inbounds %_s__String__charptr_long_long, ptr %13, i64 0, i32 2
  %.fca.2.load100 = load i64, ptr %.fca.2.gep99, align 8
  store ptr %.fca.0.load94, ptr %14, align 8
  %.fca.1.gep90 = getelementptr inbounds %_s__String__charptr_long_long, ptr %14, i64 0, i32 1
  store i64 %.fca.1.load97, ptr %.fca.1.gep90, align 8
  %.fca.2.gep92 = getelementptr inbounds %_s__String__charptr_long_long, ptr %14, i64 0, i32 2
  store i64 %.fca.2.load100, ptr %.fca.2.gep92, align 8
  %43 = call ptr @_mf__String__getRaw(ptr nonnull %14)
  %44 = call ptr @_mf__String__getRaw(ptr nonnull %3)
  call void @_mp__String__ctor__string_string(ptr nonnull %15, ptr %43, ptr %44)
  %.fca.0.load79 = load ptr, ptr %15, align 8
  %.fca.1.gep81 = getelementptr inbounds %_s__String__charptr_long_long, ptr %15, i64 0, i32 1
  %.fca.1.load82 = load i64, ptr %.fca.1.gep81, align 8
  %.fca.2.gep84 = getelementptr inbounds %_s__String__charptr_long_long, ptr %15, i64 0, i32 2
  %.fca.2.load85 = load i64, ptr %.fca.2.gep84, align 8
  store ptr %.fca.0.load79, ptr %16, align 8
  %.fca.1.gep75 = getelementptr inbounds %_s__String__charptr_long_long, ptr %16, i64 0, i32 1
  store i64 %.fca.1.load82, ptr %.fca.1.gep75, align 8
  %.fca.2.gep77 = getelementptr inbounds %_s__String__charptr_long_long, ptr %16, i64 0, i32 2
  store i64 %.fca.2.load85, ptr %.fca.2.gep77, align 8
  %45 = call ptr @_mf__String__getRaw(ptr nonnull %16)
  %46 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %45)
  %47 = call ptr @_mf__String__getRaw(ptr nonnull %3)
  call void @_mp__String__ctor__string_string(ptr nonnull %17, ptr nonnull @4, ptr %47)
  %.fca.0.load64 = load ptr, ptr %17, align 8
  %.fca.1.gep66 = getelementptr inbounds %_s__String__charptr_long_long, ptr %17, i64 0, i32 1
  %.fca.1.load67 = load i64, ptr %.fca.1.gep66, align 8
  %.fca.2.gep69 = getelementptr inbounds %_s__String__charptr_long_long, ptr %17, i64 0, i32 2
  %.fca.2.load70 = load i64, ptr %.fca.2.gep69, align 8
  store ptr %.fca.0.load64, ptr %18, align 8
  %.fca.1.gep60 = getelementptr inbounds %_s__String__charptr_long_long, ptr %18, i64 0, i32 1
  store i64 %.fca.1.load67, ptr %.fca.1.gep60, align 8
  %.fca.2.gep62 = getelementptr inbounds %_s__String__charptr_long_long, ptr %18, i64 0, i32 2
  store i64 %.fca.2.load70, ptr %.fca.2.gep62, align 8
  %48 = call ptr @_mf__String__getRaw(ptr nonnull %18)
  call void @_mp__String__ctor__string_string(ptr nonnull %19, ptr %48, ptr nonnull @5)
  %.fca.0.load49 = load ptr, ptr %19, align 8
  %.fca.1.gep51 = getelementptr inbounds %_s__String__charptr_long_long, ptr %19, i64 0, i32 1
  %.fca.1.load52 = load i64, ptr %.fca.1.gep51, align 8
  %.fca.2.gep54 = getelementptr inbounds %_s__String__charptr_long_long, ptr %19, i64 0, i32 2
  %.fca.2.load55 = load i64, ptr %.fca.2.gep54, align 8
  store ptr %.fca.0.load49, ptr %20, align 8
  %.fca.1.gep46 = getelementptr inbounds %_s__String__charptr_long_long, ptr %20, i64 0, i32 1
  store i64 %.fca.1.load52, ptr %.fca.1.gep46, align 8
  %.fca.2.gep47 = getelementptr inbounds %_s__String__charptr_long_long, ptr %20, i64 0, i32 2
  store i64 %.fca.2.load55, ptr %.fca.2.gep47, align 8
  %49 = call ptr @_mf__String__getRaw(ptr nonnull %20)
  %50 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %49)
  call void @_mp__String__ctor__string(ptr nonnull %21, ptr nonnull @14)
  call void @_mp__String__opMul__short(ptr nonnull %21, i16 4)
  %51 = call ptr @_mf__String__getRaw(ptr nonnull %21)
  %52 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %51)
  call void @_mp__String__ctor__string(ptr nonnull %22, ptr nonnull @6)
  call void @_mp__String__opMul__int(ptr nonnull %22, i32 5)
  %53 = call ptr @_mf__String__getRaw(ptr nonnull %22)
  %54 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %53)
  call void @_mp__String__ctor__char(ptr nonnull %23, i8 97)
  call void @_mp__String__opMul__int(ptr nonnull %23, i32 20)
  %55 = call ptr @_mf__String__getRaw(ptr nonnull %23)
  %56 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %55)
  call void @_mp__String__ctor__char(ptr nonnull %24, i8 99)
  call void @_mp__String__opMul__int(ptr nonnull %24, i32 2)
  call void @_mp__String__opMul__int(ptr nonnull %24, i32 7)
  %57 = call ptr @_mf__String__getRaw(ptr nonnull %24)
  %58 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %57)
  %59 = call i1 @_f__void__isRawEqual__string_string(ptr nonnull @8, ptr nonnull @9)
  %60 = zext i1 %59 to i32
  %61 = call i32 (ptr, ...) @printf(ptr nonnull @7, i32 %60)
  %62 = call i1 @_f__void__isRawEqual__string_string(ptr nonnull @12, ptr nonnull @10)
  %63 = zext i1 %62 to i32
  %64 = call i32 (ptr, ...) @printf(ptr nonnull @7, i32 %63)
  %65 = call i1 @_f__void__isRawEqual__string_string(ptr nonnull @12, ptr nonnull @12)
  %66 = zext i1 %65 to i32
  %67 = call i32 (ptr, ...) @printf(ptr nonnull @7, i32 %66)
  %68 = call i1 @_f__void__isRawEqual__string_string(ptr nonnull @8, ptr nonnull @9)
  %69 = xor i1 %68, true
  %70 = zext i1 %69 to i32
  %71 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %70)
  %72 = call i1 @_f__void__isRawEqual__string_string(ptr nonnull @12, ptr nonnull @10)
  %73 = xor i1 %72, true
  %74 = zext i1 %73 to i32
  %75 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %74)
  %76 = call i1 @_f__void__isRawEqual__string_string(ptr nonnull @12, ptr nonnull @12)
  %77 = xor i1 %76, true
  %78 = zext i1 %77 to i32
  %79 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %78)
  call void @_mp__String__ctor__string(ptr nonnull %25, ptr nonnull @12)
  call void @_mp__String__append__char(ptr nonnull %25, i8 108)
  %80 = call ptr @_mf__String__getRaw(ptr nonnull %25)
  %81 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %80)
  call void @_mp__String__ctor__string(ptr nonnull %26, ptr nonnull @14)
  call void @_mp__String__append__string(ptr nonnull %26, ptr nonnull @13)
  %82 = call ptr @_mf__String__getRaw(ptr nonnull %26)
  %83 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %82)
  call void @_mp__String__ctor__string(ptr nonnull %27, ptr nonnull @14)
  call void @_mp__String__opMul__int(ptr nonnull %27, i32 3)
  %84 = call ptr @_mf__String__getRaw(ptr nonnull %27)
  %85 = call i32 (ptr, ...) @printf(ptr nonnull @15, ptr %84)
  call void @_mp__String__dtor(ptr nonnull %17)
  call void @_mp__String__dtor(ptr nonnull %19)
  call void @_mp__String__dtor(ptr nonnull %21)
  call void @_mp__String__dtor(ptr nonnull %22)
  call void @_mp__String__dtor(ptr nonnull %23)
  call void @_mp__String__dtor(ptr nonnull %24)
  call void @_mp__String__dtor(ptr nonnull %25)
  call void @_mp__String__dtor(ptr nonnull %26)
  call void @_mp__String__dtor(ptr nonnull %0)
  call void @_mp__String__dtor(ptr nonnull %27)
  call void @_mp__String__dtor(ptr nonnull %2)
  call void @_mp__String__dtor(ptr nonnull %5)
  call void @_mp__String__dtor(ptr nonnull %7)
  call void @_mp__String__dtor(ptr nonnull %10)
  call void @_mp__String__dtor(ptr nonnull %13)
  call void @_mp__String__dtor(ptr nonnull %15)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare void @_mp__String__ctor__string_string(ptr, ptr, ptr) local_unnamed_addr

declare ptr @_mf__String__getRaw(ptr) local_unnamed_addr

declare void @_mp__String__ctor__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__opMul__short(ptr, i16) local_unnamed_addr

declare void @_mp__String__opMul__int(ptr, i32) local_unnamed_addr

declare void @_mp__String__ctor__char(ptr, i8) local_unnamed_addr

declare i1 @_f__void__isRawEqual__string_string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__append__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__append__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__dtor(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
