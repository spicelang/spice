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
@7 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@8 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@9 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@10 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@11 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@12 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@13 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@14 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@15 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@16 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@17 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l1:
  %0 = alloca %_s__String__charptr_long_long, align 8
  %1 = alloca %_s__String__charptr_long_long, align 8
  %2 = alloca %_s__String__charptr_long_long, align 8
  %3 = alloca %_s__String__charptr_long_long, align 8
  %s1 = alloca %_s__String__charptr_long_long, align 8
  %s12 = alloca %_s__String__charptr_long_long, align 8
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
  %s2 = alloca %_s__String__charptr_long_long, align 8
  %s25 = alloca %_s__String__charptr_long_long, align 8
  %23 = alloca %_s__String__charptr_long_long, align 8
  %24 = alloca %_s__String__charptr_long_long, align 8
  %25 = alloca %_s__String__charptr_long_long, align 8
  %26 = alloca %_s__String__charptr_long_long, align 8
  %s3 = alloca %_s__String__charptr_long_long, align 8
  %s310 = alloca %_s__String__charptr_long_long, align 8
  %27 = alloca %_s__String__charptr_long_long, align 8
  %28 = alloca %_s__String__charptr_long_long, align 8
  %29 = alloca %_s__String__charptr_long_long, align 8
  %30 = alloca %_s__String__charptr_long_long, align 8
  %31 = alloca %_s__String__charptr_long_long, align 8
  %32 = alloca %_s__String__charptr_long_long, align 8
  %33 = alloca %_s__String__charptr_long_long, align 8
  %34 = alloca %_s__String__charptr_long_long, align 8
  %35 = alloca %_s__String__charptr_long_long, align 8
  %36 = alloca %_s__String__charptr_long_long, align 8
  %37 = alloca %_s__String__charptr_long_long, align 8
  %38 = alloca %_s__String__charptr_long_long, align 8
  %39 = alloca %_s__String__charptr_long_long, align 8
  %40 = alloca %_s__String__charptr_long_long, align 8
  %s4 = alloca %_s__String__charptr_long_long, align 8
  %s5 = alloca %_s__String__charptr_long_long, align 8
  %s6 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %0, ptr nonnull @6)
  %.fca.0.load488 = load ptr, ptr %0, align 8
  %.fca.0.insert489 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load488, 0
  %.fca.1.gep490 = getelementptr inbounds %_s__String__charptr_long_long, ptr %0, i64 0, i32 1
  %.fca.1.load491 = load i64, ptr %.fca.1.gep490, align 8
  %.fca.1.insert492 = insertvalue %_s__String__charptr_long_long %.fca.0.insert489, i64 %.fca.1.load491, 1
  %.fca.2.gep493 = getelementptr inbounds %_s__String__charptr_long_long, ptr %0, i64 0, i32 2
  %.fca.2.load494 = load i64, ptr %.fca.2.gep493, align 8
  %.fca.2.insert495 = insertvalue %_s__String__charptr_long_long %.fca.1.insert492, i64 %.fca.2.load494, 2
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @0)
  %.fca.0.load478 = load ptr, ptr %1, align 8
  %.fca.0.insert479 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load478, 0
  %.fca.1.gep480 = getelementptr inbounds %_s__String__charptr_long_long, ptr %1, i64 0, i32 1
  %.fca.1.load481 = load i64, ptr %.fca.1.gep480, align 8
  %.fca.1.insert482 = insertvalue %_s__String__charptr_long_long %.fca.0.insert479, i64 %.fca.1.load481, 1
  %.fca.2.gep483 = getelementptr inbounds %_s__String__charptr_long_long, ptr %1, i64 0, i32 2
  %.fca.2.load484 = load i64, ptr %.fca.2.gep483, align 8
  %.fca.2.insert485 = insertvalue %_s__String__charptr_long_long %.fca.1.insert482, i64 %.fca.2.load484, 2
  call void @_mp__String__void__ctor__String_String(ptr nonnull %2, %_s__String__charptr_long_long %.fca.2.insert495, %_s__String__charptr_long_long %.fca.2.insert485)
  %.fca.0.load468 = load ptr, ptr %2, align 8
  %.fca.1.gep470 = getelementptr inbounds %_s__String__charptr_long_long, ptr %2, i64 0, i32 1
  %.fca.1.load471 = load i64, ptr %.fca.1.gep470, align 8
  %.fca.2.gep473 = getelementptr inbounds %_s__String__charptr_long_long, ptr %2, i64 0, i32 2
  %.fca.2.load474 = load i64, ptr %.fca.2.gep473, align 8
  store ptr %.fca.0.load468, ptr %3, align 8
  %.fca.1.gep464 = getelementptr inbounds %_s__String__charptr_long_long, ptr %3, i64 0, i32 1
  store i64 %.fca.1.load471, ptr %.fca.1.gep464, align 8
  %.fca.2.gep466 = getelementptr inbounds %_s__String__charptr_long_long, ptr %3, i64 0, i32 2
  store i64 %.fca.2.load474, ptr %.fca.2.gep466, align 8
  %41 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %3)
  %42 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %41)
  call void @_mp__String__void__ctor__string(ptr nonnull %s1, ptr nonnull @6)
  %.fca.0.load453 = load ptr, ptr %s1, align 8
  %.fca.0.insert454 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load453, 0
  %.fca.1.gep455 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s1, i64 0, i32 1
  %.fca.1.load456 = load i64, ptr %.fca.1.gep455, align 8
  %.fca.1.insert457 = insertvalue %_s__String__charptr_long_long %.fca.0.insert454, i64 %.fca.1.load456, 1
  %.fca.2.gep458 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s1, i64 0, i32 2
  %.fca.2.load459 = load i64, ptr %.fca.2.gep458, align 8
  %.fca.2.insert460 = insertvalue %_s__String__charptr_long_long %.fca.1.insert457, i64 %.fca.2.load459, 2
  call void @_mp__String__void__ctor__string(ptr nonnull %s12, ptr nonnull @0)
  %.fca.0.load444 = load ptr, ptr %s12, align 8
  %.fca.0.insert445 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load444, 0
  %.fca.1.gep446 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s12, i64 0, i32 1
  %.fca.1.load447 = load i64, ptr %.fca.1.gep446, align 8
  %.fca.1.insert448 = insertvalue %_s__String__charptr_long_long %.fca.0.insert445, i64 %.fca.1.load447, 1
  %.fca.2.gep449 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s12, i64 0, i32 2
  %.fca.2.load450 = load i64, ptr %.fca.2.gep449, align 8
  %.fca.2.insert451 = insertvalue %_s__String__charptr_long_long %.fca.1.insert448, i64 %.fca.2.load450, 2
  call void @_mp__String__void__ctor__String_String(ptr nonnull %4, %_s__String__charptr_long_long %.fca.2.insert460, %_s__String__charptr_long_long %.fca.2.insert451)
  %.fca.0.load435 = load ptr, ptr %4, align 8
  %.fca.1.gep437 = getelementptr inbounds %_s__String__charptr_long_long, ptr %4, i64 0, i32 1
  %.fca.1.load438 = load i64, ptr %.fca.1.gep437, align 8
  %.fca.2.gep440 = getelementptr inbounds %_s__String__charptr_long_long, ptr %4, i64 0, i32 2
  %.fca.2.load441 = load i64, ptr %.fca.2.gep440, align 8
  store ptr %.fca.0.load435, ptr %5, align 8
  %.fca.1.gep368 = getelementptr inbounds %_s__String__charptr_long_long, ptr %5, i64 0, i32 1
  store i64 %.fca.1.load438, ptr %.fca.1.gep368, align 8
  %.fca.2.gep370 = getelementptr inbounds %_s__String__charptr_long_long, ptr %5, i64 0, i32 2
  store i64 %.fca.2.load441, ptr %.fca.2.gep370, align 8
  %43 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %5)
  %44 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %43)
  %.fca.0.load372 = load ptr, ptr %5, align 8
  %.fca.0.insert373 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load372, 0
  %.fca.1.load375 = load i64, ptr %.fca.1.gep368, align 8
  %.fca.1.insert376 = insertvalue %_s__String__charptr_long_long %.fca.0.insert373, i64 %.fca.1.load375, 1
  %.fca.2.load378 = load i64, ptr %.fca.2.gep370, align 8
  %.fca.2.insert379 = insertvalue %_s__String__charptr_long_long %.fca.1.insert376, i64 %.fca.2.load378, 2
  call void @_mp__String__void__ctor__String_string(ptr nonnull %6, %_s__String__charptr_long_long %.fca.2.insert379, ptr nonnull @1)
  %.fca.0.load357 = load ptr, ptr %6, align 8
  %.fca.1.gep359 = getelementptr inbounds %_s__String__charptr_long_long, ptr %6, i64 0, i32 1
  %.fca.1.load360 = load i64, ptr %.fca.1.gep359, align 8
  %.fca.2.gep362 = getelementptr inbounds %_s__String__charptr_long_long, ptr %6, i64 0, i32 2
  %.fca.2.load363 = load i64, ptr %.fca.2.gep362, align 8
  store ptr %.fca.0.load357, ptr %7, align 8
  %.fca.1.gep353 = getelementptr inbounds %_s__String__charptr_long_long, ptr %7, i64 0, i32 1
  store i64 %.fca.1.load360, ptr %.fca.1.gep353, align 8
  %.fca.2.gep355 = getelementptr inbounds %_s__String__charptr_long_long, ptr %7, i64 0, i32 2
  store i64 %.fca.2.load363, ptr %.fca.2.gep355, align 8
  %45 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %7)
  %46 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %45)
  call void @_mp__String__void__ctor__string(ptr nonnull %8, ptr nonnull @2)
  %.fca.0.load342 = load ptr, ptr %8, align 8
  %.fca.0.insert343 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load342, 0
  %.fca.1.gep344 = getelementptr inbounds %_s__String__charptr_long_long, ptr %8, i64 0, i32 1
  %.fca.1.load345 = load i64, ptr %.fca.1.gep344, align 8
  %.fca.1.insert346 = insertvalue %_s__String__charptr_long_long %.fca.0.insert343, i64 %.fca.1.load345, 1
  %.fca.2.gep347 = getelementptr inbounds %_s__String__charptr_long_long, ptr %8, i64 0, i32 2
  %.fca.2.load348 = load i64, ptr %.fca.2.gep347, align 8
  %.fca.2.insert349 = insertvalue %_s__String__charptr_long_long %.fca.1.insert346, i64 %.fca.2.load348, 2
  %.fca.0.load381 = load ptr, ptr %5, align 8
  %.fca.0.insert382 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load381, 0
  %.fca.1.load384 = load i64, ptr %.fca.1.gep368, align 8
  %.fca.1.insert385 = insertvalue %_s__String__charptr_long_long %.fca.0.insert382, i64 %.fca.1.load384, 1
  %.fca.2.load387 = load i64, ptr %.fca.2.gep370, align 8
  %.fca.2.insert388 = insertvalue %_s__String__charptr_long_long %.fca.1.insert385, i64 %.fca.2.load387, 2
  call void @_mp__String__void__ctor__String_String(ptr nonnull %9, %_s__String__charptr_long_long %.fca.2.insert349, %_s__String__charptr_long_long %.fca.2.insert388)
  %.fca.0.load332 = load ptr, ptr %9, align 8
  %.fca.1.gep334 = getelementptr inbounds %_s__String__charptr_long_long, ptr %9, i64 0, i32 1
  %.fca.1.load335 = load i64, ptr %.fca.1.gep334, align 8
  %.fca.2.gep337 = getelementptr inbounds %_s__String__charptr_long_long, ptr %9, i64 0, i32 2
  %.fca.2.load338 = load i64, ptr %.fca.2.gep337, align 8
  store ptr %.fca.0.load332, ptr %10, align 8
  %.fca.1.gep328 = getelementptr inbounds %_s__String__charptr_long_long, ptr %10, i64 0, i32 1
  store i64 %.fca.1.load335, ptr %.fca.1.gep328, align 8
  %.fca.2.gep330 = getelementptr inbounds %_s__String__charptr_long_long, ptr %10, i64 0, i32 2
  store i64 %.fca.2.load338, ptr %.fca.2.gep330, align 8
  %47 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %10)
  %48 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %47)
  %.fca.0.load390 = load ptr, ptr %5, align 8
  %.fca.0.insert391 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load390, 0
  %.fca.1.load393 = load i64, ptr %.fca.1.gep368, align 8
  %.fca.1.insert394 = insertvalue %_s__String__charptr_long_long %.fca.0.insert391, i64 %.fca.1.load393, 1
  %.fca.2.load396 = load i64, ptr %.fca.2.gep370, align 8
  %.fca.2.insert397 = insertvalue %_s__String__charptr_long_long %.fca.1.insert394, i64 %.fca.2.load396, 2
  call void @_mp__String__void__ctor__String_String(ptr nonnull %11, %_s__String__charptr_long_long %.fca.2.insert397, %_s__String__charptr_long_long %.fca.2.insert397)
  %.fca.0.load317 = load ptr, ptr %11, align 8
  %.fca.1.gep319 = getelementptr inbounds %_s__String__charptr_long_long, ptr %11, i64 0, i32 1
  %.fca.1.load320 = load i64, ptr %.fca.1.gep319, align 8
  %.fca.2.gep322 = getelementptr inbounds %_s__String__charptr_long_long, ptr %11, i64 0, i32 2
  %.fca.2.load323 = load i64, ptr %.fca.2.gep322, align 8
  store ptr %.fca.0.load317, ptr %12, align 8
  %.fca.1.gep313 = getelementptr inbounds %_s__String__charptr_long_long, ptr %12, i64 0, i32 1
  store i64 %.fca.1.load320, ptr %.fca.1.gep313, align 8
  %.fca.2.gep315 = getelementptr inbounds %_s__String__charptr_long_long, ptr %12, i64 0, i32 2
  store i64 %.fca.2.load323, ptr %.fca.2.gep315, align 8
  %49 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %12)
  %50 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %49)
  %.fca.0.load408 = load ptr, ptr %5, align 8
  %.fca.0.insert409 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load408, 0
  %.fca.1.load411 = load i64, ptr %.fca.1.gep368, align 8
  %.fca.1.insert412 = insertvalue %_s__String__charptr_long_long %.fca.0.insert409, i64 %.fca.1.load411, 1
  %.fca.2.load414 = load i64, ptr %.fca.2.gep370, align 8
  %.fca.2.insert415 = insertvalue %_s__String__charptr_long_long %.fca.1.insert412, i64 %.fca.2.load414, 2
  call void @_mp__String__void__ctor__String_string(ptr nonnull %13, %_s__String__charptr_long_long %.fca.2.insert415, ptr nonnull @3)
  %.fca.0.load302 = load ptr, ptr %13, align 8
  %.fca.0.insert303 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load302, 0
  %.fca.1.gep304 = getelementptr inbounds %_s__String__charptr_long_long, ptr %13, i64 0, i32 1
  %.fca.1.load305 = load i64, ptr %.fca.1.gep304, align 8
  %.fca.1.insert306 = insertvalue %_s__String__charptr_long_long %.fca.0.insert303, i64 %.fca.1.load305, 1
  %.fca.2.gep307 = getelementptr inbounds %_s__String__charptr_long_long, ptr %13, i64 0, i32 2
  %.fca.2.load308 = load i64, ptr %.fca.2.gep307, align 8
  %.fca.2.insert309 = insertvalue %_s__String__charptr_long_long %.fca.1.insert306, i64 %.fca.2.load308, 2
  %.fca.0.load417 = load ptr, ptr %5, align 8
  %.fca.0.insert418 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load417, 0
  %.fca.1.load420 = load i64, ptr %.fca.1.gep368, align 8
  %.fca.1.insert421 = insertvalue %_s__String__charptr_long_long %.fca.0.insert418, i64 %.fca.1.load420, 1
  %.fca.2.load423 = load i64, ptr %.fca.2.gep370, align 8
  %.fca.2.insert424 = insertvalue %_s__String__charptr_long_long %.fca.1.insert421, i64 %.fca.2.load423, 2
  call void @_mp__String__void__ctor__String_String(ptr nonnull %14, %_s__String__charptr_long_long %.fca.2.insert309, %_s__String__charptr_long_long %.fca.2.insert424)
  %.fca.0.load293 = load ptr, ptr %14, align 8
  %.fca.1.gep295 = getelementptr inbounds %_s__String__charptr_long_long, ptr %14, i64 0, i32 1
  %.fca.1.load296 = load i64, ptr %.fca.1.gep295, align 8
  %.fca.2.gep298 = getelementptr inbounds %_s__String__charptr_long_long, ptr %14, i64 0, i32 2
  %.fca.2.load299 = load i64, ptr %.fca.2.gep298, align 8
  store ptr %.fca.0.load293, ptr %15, align 8
  %.fca.1.gep289 = getelementptr inbounds %_s__String__charptr_long_long, ptr %15, i64 0, i32 1
  store i64 %.fca.1.load296, ptr %.fca.1.gep289, align 8
  %.fca.2.gep291 = getelementptr inbounds %_s__String__charptr_long_long, ptr %15, i64 0, i32 2
  store i64 %.fca.2.load299, ptr %.fca.2.gep291, align 8
  %51 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %15)
  %52 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %51)
  call void @_mp__String__void__ctor__string(ptr nonnull %16, ptr nonnull @4)
  %.fca.0.load278 = load ptr, ptr %16, align 8
  %.fca.0.insert279 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load278, 0
  %.fca.1.gep280 = getelementptr inbounds %_s__String__charptr_long_long, ptr %16, i64 0, i32 1
  %.fca.1.load281 = load i64, ptr %.fca.1.gep280, align 8
  %.fca.1.insert282 = insertvalue %_s__String__charptr_long_long %.fca.0.insert279, i64 %.fca.1.load281, 1
  %.fca.2.gep283 = getelementptr inbounds %_s__String__charptr_long_long, ptr %16, i64 0, i32 2
  %.fca.2.load284 = load i64, ptr %.fca.2.gep283, align 8
  %.fca.2.insert285 = insertvalue %_s__String__charptr_long_long %.fca.1.insert282, i64 %.fca.2.load284, 2
  %.fca.0.load426 = load ptr, ptr %5, align 8
  %.fca.0.insert427 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load426, 0
  %.fca.1.load429 = load i64, ptr %.fca.1.gep368, align 8
  %.fca.1.insert430 = insertvalue %_s__String__charptr_long_long %.fca.0.insert427, i64 %.fca.1.load429, 1
  %.fca.2.load432 = load i64, ptr %.fca.2.gep370, align 8
  %.fca.2.insert433 = insertvalue %_s__String__charptr_long_long %.fca.1.insert430, i64 %.fca.2.load432, 2
  call void @_mp__String__void__ctor__String_String(ptr nonnull %17, %_s__String__charptr_long_long %.fca.2.insert285, %_s__String__charptr_long_long %.fca.2.insert433)
  %.fca.0.load268 = load ptr, ptr %17, align 8
  %.fca.0.insert269 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load268, 0
  %.fca.1.gep270 = getelementptr inbounds %_s__String__charptr_long_long, ptr %17, i64 0, i32 1
  %.fca.1.load271 = load i64, ptr %.fca.1.gep270, align 8
  %.fca.1.insert272 = insertvalue %_s__String__charptr_long_long %.fca.0.insert269, i64 %.fca.1.load271, 1
  %.fca.2.gep273 = getelementptr inbounds %_s__String__charptr_long_long, ptr %17, i64 0, i32 2
  %.fca.2.load274 = load i64, ptr %.fca.2.gep273, align 8
  %.fca.2.insert275 = insertvalue %_s__String__charptr_long_long %.fca.1.insert272, i64 %.fca.2.load274, 2
  call void @_mp__String__void__ctor__String_string(ptr nonnull %18, %_s__String__charptr_long_long %.fca.2.insert275, ptr nonnull @5)
  %.fca.0.load259 = load ptr, ptr %18, align 8
  %.fca.1.gep261 = getelementptr inbounds %_s__String__charptr_long_long, ptr %18, i64 0, i32 1
  %.fca.1.load262 = load i64, ptr %.fca.1.gep261, align 8
  %.fca.2.gep264 = getelementptr inbounds %_s__String__charptr_long_long, ptr %18, i64 0, i32 2
  %.fca.2.load265 = load i64, ptr %.fca.2.gep264, align 8
  store ptr %.fca.0.load259, ptr %19, align 8
  %.fca.1.gep256 = getelementptr inbounds %_s__String__charptr_long_long, ptr %19, i64 0, i32 1
  store i64 %.fca.1.load262, ptr %.fca.1.gep256, align 8
  %.fca.2.gep257 = getelementptr inbounds %_s__String__charptr_long_long, ptr %19, i64 0, i32 2
  store i64 %.fca.2.load265, ptr %.fca.2.gep257, align 8
  %53 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %19)
  %54 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %53)
  call void @_mp__String__void__ctor__string(ptr nonnull %20, ptr nonnull @16)
  %.fca.0.load247 = load ptr, ptr %20, align 8
  %.fca.0.insert248 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load247, 0
  %.fca.1.gep249 = getelementptr inbounds %_s__String__charptr_long_long, ptr %20, i64 0, i32 1
  %.fca.1.load250 = load i64, ptr %.fca.1.gep249, align 8
  %.fca.1.insert251 = insertvalue %_s__String__charptr_long_long %.fca.0.insert248, i64 %.fca.1.load250, 1
  %.fca.2.gep252 = getelementptr inbounds %_s__String__charptr_long_long, ptr %20, i64 0, i32 2
  %.fca.2.load253 = load i64, ptr %.fca.2.gep252, align 8
  %.fca.2.insert254 = insertvalue %_s__String__charptr_long_long %.fca.1.insert251, i64 %.fca.2.load253, 2
  call void @_mp__String__void__ctor__string(ptr nonnull %21, ptr nonnull @16)
  call void @_mp__String__void__ctor__String(ptr nonnull %22, %_s__String__charptr_long_long %.fca.2.insert254)
  call void @_mp__String__void__opMul__short(ptr nonnull %22, i16 4)
  %55 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %22)
  %56 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %55)
  call void @_mp__String__void__ctor__string(ptr nonnull %s2, ptr nonnull @6)
  %.fca.0.load227 = load ptr, ptr %s2, align 8
  %.fca.0.insert228 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load227, 0
  %.fca.1.gep229 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s2, i64 0, i32 1
  %.fca.1.load230 = load i64, ptr %.fca.1.gep229, align 8
  %.fca.1.insert231 = insertvalue %_s__String__charptr_long_long %.fca.0.insert228, i64 %.fca.1.load230, 1
  %.fca.2.gep232 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s2, i64 0, i32 2
  %.fca.2.load233 = load i64, ptr %.fca.2.gep232, align 8
  %.fca.2.insert234 = insertvalue %_s__String__charptr_long_long %.fca.1.insert231, i64 %.fca.2.load233, 2
  call void @_mp__String__void__ctor__string(ptr nonnull %s25, ptr nonnull @6)
  call void @_mp__String__void__ctor__String(ptr nonnull %23, %_s__String__charptr_long_long %.fca.2.insert234)
  call void @_mp__String__void__opMul__int(ptr nonnull %23, i32 5)
  %57 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %23)
  %58 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %57)
  call void @_mp__String__void__ctor__char(ptr nonnull %24, i8 97)
  %.fca.0.load209 = load ptr, ptr %24, align 8
  %.fca.0.insert210 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load209, 0
  %.fca.1.gep211 = getelementptr inbounds %_s__String__charptr_long_long, ptr %24, i64 0, i32 1
  %.fca.1.load212 = load i64, ptr %.fca.1.gep211, align 8
  %.fca.1.insert213 = insertvalue %_s__String__charptr_long_long %.fca.0.insert210, i64 %.fca.1.load212, 1
  %.fca.2.gep214 = getelementptr inbounds %_s__String__charptr_long_long, ptr %24, i64 0, i32 2
  %.fca.2.load215 = load i64, ptr %.fca.2.gep214, align 8
  %.fca.2.insert216 = insertvalue %_s__String__charptr_long_long %.fca.1.insert213, i64 %.fca.2.load215, 2
  call void @_mp__String__void__ctor__char(ptr nonnull %25, i8 97)
  call void @_mp__String__void__ctor__String(ptr nonnull %26, %_s__String__charptr_long_long %.fca.2.insert216)
  call void @_mp__String__void__opMul__int(ptr nonnull %26, i32 20)
  %59 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %26)
  %60 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %59)
  call void @_mp__String__void__ctor__char(ptr nonnull %s3, i8 99)
  %.fca.0.load190 = load ptr, ptr %s3, align 8
  %.fca.0.insert191 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load190, 0
  %.fca.1.gep192 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s3, i64 0, i32 1
  %.fca.1.load193 = load i64, ptr %.fca.1.gep192, align 8
  %.fca.1.insert194 = insertvalue %_s__String__charptr_long_long %.fca.0.insert191, i64 %.fca.1.load193, 1
  %.fca.2.gep195 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s3, i64 0, i32 2
  %.fca.2.load196 = load i64, ptr %.fca.2.gep195, align 8
  %.fca.2.insert197 = insertvalue %_s__String__charptr_long_long %.fca.1.insert194, i64 %.fca.2.load196, 2
  call void @_mp__String__void__ctor__char(ptr nonnull %s310, i8 99)
  call void @_mp__String__void__ctor__String(ptr nonnull %27, %_s__String__charptr_long_long %.fca.2.insert197)
  call void @_mp__String__void__opMul__int(ptr nonnull %27, i32 2)
  %.fca.0.load181 = load ptr, ptr %27, align 8
  %.fca.0.insert182 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load181, 0
  %.fca.1.gep183 = getelementptr inbounds %_s__String__charptr_long_long, ptr %27, i64 0, i32 1
  %.fca.1.load184 = load i64, ptr %.fca.1.gep183, align 8
  %.fca.1.insert185 = insertvalue %_s__String__charptr_long_long %.fca.0.insert182, i64 %.fca.1.load184, 1
  %.fca.2.gep186 = getelementptr inbounds %_s__String__charptr_long_long, ptr %27, i64 0, i32 2
  %.fca.2.load187 = load i64, ptr %.fca.2.gep186, align 8
  %.fca.2.insert188 = insertvalue %_s__String__charptr_long_long %.fca.1.insert185, i64 %.fca.2.load187, 2
  call void @_mp__String__void__ctor__String(ptr nonnull %28, %_s__String__charptr_long_long %.fca.2.insert188)
  call void @_mp__String__void__opMul__int(ptr nonnull %28, i32 7)
  %61 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %28)
  %62 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %61)
  %63 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @10, ptr nonnull @11)
  %64 = zext i1 %63 to i32
  %65 = call i32 (ptr, ...) @printf(ptr nonnull @7, i32 %64)
  %66 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @14, ptr nonnull @12)
  %67 = zext i1 %66 to i32
  %68 = call i32 (ptr, ...) @printf(ptr nonnull @7, i32 %67)
  %69 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @14, ptr nonnull @14)
  %70 = zext i1 %69 to i32
  %71 = call i32 (ptr, ...) @printf(ptr nonnull @7, i32 %70)
  call void @_mp__String__void__ctor__string(ptr nonnull %29, ptr nonnull @10)
  call void @_mp__String__void__ctor__string(ptr nonnull %30, ptr nonnull @11)
  %.fca.0.load147 = load ptr, ptr %30, align 8
  %.fca.0.insert148 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load147, 0
  %.fca.1.gep149 = getelementptr inbounds %_s__String__charptr_long_long, ptr %30, i64 0, i32 1
  %.fca.1.load150 = load i64, ptr %.fca.1.gep149, align 8
  %.fca.1.insert151 = insertvalue %_s__String__charptr_long_long %.fca.0.insert148, i64 %.fca.1.load150, 1
  %.fca.2.gep152 = getelementptr inbounds %_s__String__charptr_long_long, ptr %30, i64 0, i32 2
  %.fca.2.load153 = load i64, ptr %.fca.2.gep152, align 8
  %.fca.2.insert154 = insertvalue %_s__String__charptr_long_long %.fca.1.insert151, i64 %.fca.2.load153, 2
  %72 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %29, %_s__String__charptr_long_long %.fca.2.insert154)
  %73 = zext i1 %72 to i32
  %74 = call i32 (ptr, ...) @printf(ptr nonnull @8, i32 %73)
  call void @_mp__String__void__ctor__string(ptr nonnull %31, ptr nonnull @14)
  call void @_mp__String__void__ctor__string(ptr nonnull %32, ptr nonnull @12)
  %.fca.0.load126 = load ptr, ptr %32, align 8
  %.fca.0.insert127 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load126, 0
  %.fca.1.gep128 = getelementptr inbounds %_s__String__charptr_long_long, ptr %32, i64 0, i32 1
  %.fca.1.load129 = load i64, ptr %.fca.1.gep128, align 8
  %.fca.1.insert130 = insertvalue %_s__String__charptr_long_long %.fca.0.insert127, i64 %.fca.1.load129, 1
  %.fca.2.gep131 = getelementptr inbounds %_s__String__charptr_long_long, ptr %32, i64 0, i32 2
  %.fca.2.load132 = load i64, ptr %.fca.2.gep131, align 8
  %.fca.2.insert133 = insertvalue %_s__String__charptr_long_long %.fca.1.insert130, i64 %.fca.2.load132, 2
  %75 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %31, %_s__String__charptr_long_long %.fca.2.insert133)
  %76 = zext i1 %75 to i32
  %77 = call i32 (ptr, ...) @printf(ptr nonnull @8, i32 %76)
  call void @_mp__String__void__ctor__string(ptr nonnull %33, ptr nonnull @14)
  call void @_mp__String__void__ctor__string(ptr nonnull %34, ptr nonnull @14)
  %.fca.0.load105 = load ptr, ptr %34, align 8
  %.fca.0.insert106 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load105, 0
  %.fca.1.gep107 = getelementptr inbounds %_s__String__charptr_long_long, ptr %34, i64 0, i32 1
  %.fca.1.load108 = load i64, ptr %.fca.1.gep107, align 8
  %.fca.1.insert109 = insertvalue %_s__String__charptr_long_long %.fca.0.insert106, i64 %.fca.1.load108, 1
  %.fca.2.gep110 = getelementptr inbounds %_s__String__charptr_long_long, ptr %34, i64 0, i32 2
  %.fca.2.load111 = load i64, ptr %.fca.2.gep110, align 8
  %.fca.2.insert112 = insertvalue %_s__String__charptr_long_long %.fca.1.insert109, i64 %.fca.2.load111, 2
  %78 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %33, %_s__String__charptr_long_long %.fca.2.insert112)
  %79 = zext i1 %78 to i32
  %80 = call i32 (ptr, ...) @printf(ptr nonnull @8, i32 %79)
  %81 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @10, ptr nonnull @11)
  %82 = xor i1 %81, true
  %83 = zext i1 %82 to i32
  %84 = call i32 (ptr, ...) @printf(ptr nonnull @9, i32 %83)
  %85 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @14, ptr nonnull @12)
  %86 = xor i1 %85, true
  %87 = zext i1 %86 to i32
  %88 = call i32 (ptr, ...) @printf(ptr nonnull @9, i32 %87)
  %89 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @14, ptr nonnull @14)
  %90 = xor i1 %89, true
  %91 = zext i1 %90 to i32
  %92 = call i32 (ptr, ...) @printf(ptr nonnull @9, i32 %91)
  call void @_mp__String__void__ctor__string(ptr nonnull %35, ptr nonnull @10)
  call void @_mp__String__void__ctor__string(ptr nonnull %36, ptr nonnull @11)
  %.fca.0.load78 = load ptr, ptr %36, align 8
  %.fca.0.insert79 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load78, 0
  %.fca.1.gep80 = getelementptr inbounds %_s__String__charptr_long_long, ptr %36, i64 0, i32 1
  %.fca.1.load81 = load i64, ptr %.fca.1.gep80, align 8
  %.fca.1.insert82 = insertvalue %_s__String__charptr_long_long %.fca.0.insert79, i64 %.fca.1.load81, 1
  %.fca.2.gep83 = getelementptr inbounds %_s__String__charptr_long_long, ptr %36, i64 0, i32 2
  %.fca.2.load84 = load i64, ptr %.fca.2.gep83, align 8
  %.fca.2.insert85 = insertvalue %_s__String__charptr_long_long %.fca.1.insert82, i64 %.fca.2.load84, 2
  %93 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %35, %_s__String__charptr_long_long %.fca.2.insert85)
  %94 = xor i1 %93, true
  %95 = zext i1 %94 to i32
  %96 = call i32 (ptr, ...) @printf(ptr nonnull @13, i32 %95)
  call void @_mp__String__void__ctor__string(ptr nonnull %37, ptr nonnull @14)
  call void @_mp__String__void__ctor__string(ptr nonnull %38, ptr nonnull @12)
  %.fca.0.load57 = load ptr, ptr %38, align 8
  %.fca.0.insert58 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load57, 0
  %.fca.1.gep59 = getelementptr inbounds %_s__String__charptr_long_long, ptr %38, i64 0, i32 1
  %.fca.1.load60 = load i64, ptr %.fca.1.gep59, align 8
  %.fca.1.insert61 = insertvalue %_s__String__charptr_long_long %.fca.0.insert58, i64 %.fca.1.load60, 1
  %.fca.2.gep62 = getelementptr inbounds %_s__String__charptr_long_long, ptr %38, i64 0, i32 2
  %.fca.2.load63 = load i64, ptr %.fca.2.gep62, align 8
  %.fca.2.insert64 = insertvalue %_s__String__charptr_long_long %.fca.1.insert61, i64 %.fca.2.load63, 2
  %97 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %37, %_s__String__charptr_long_long %.fca.2.insert64)
  %98 = xor i1 %97, true
  %99 = zext i1 %98 to i32
  %100 = call i32 (ptr, ...) @printf(ptr nonnull @13, i32 %99)
  call void @_mp__String__void__ctor__string(ptr nonnull %39, ptr nonnull @14)
  call void @_mp__String__void__ctor__string(ptr nonnull %40, ptr nonnull @14)
  %.fca.0.load36 = load ptr, ptr %40, align 8
  %.fca.0.insert37 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load36, 0
  %.fca.1.gep38 = getelementptr inbounds %_s__String__charptr_long_long, ptr %40, i64 0, i32 1
  %.fca.1.load39 = load i64, ptr %.fca.1.gep38, align 8
  %.fca.1.insert40 = insertvalue %_s__String__charptr_long_long %.fca.0.insert37, i64 %.fca.1.load39, 1
  %.fca.2.gep41 = getelementptr inbounds %_s__String__charptr_long_long, ptr %40, i64 0, i32 2
  %.fca.2.load42 = load i64, ptr %.fca.2.gep41, align 8
  %.fca.2.insert43 = insertvalue %_s__String__charptr_long_long %.fca.1.insert40, i64 %.fca.2.load42, 2
  %101 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %39, %_s__String__charptr_long_long %.fca.2.insert43)
  %102 = xor i1 %101, true
  %103 = zext i1 %102 to i32
  %104 = call i32 (ptr, ...) @printf(ptr nonnull @13, i32 %103)
  call void @_mp__String__void__ctor__string(ptr nonnull %s4, ptr nonnull @14)
  call void @_mp__String__void__append__char(ptr nonnull %s4, i8 108)
  %105 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s4)
  %106 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %105)
  call void @_mp__String__void__ctor__string(ptr nonnull %s5, ptr nonnull @16)
  call void @_mp__String__void__append__string(ptr nonnull %s5, ptr nonnull @15)
  %107 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s5)
  %108 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %107)
  call void @_mp__String__void__ctor__string(ptr nonnull %s6, ptr nonnull @16)
  call void @_mp__String__void__opMul__int(ptr nonnull %s6, i32 3)
  %109 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s6)
  %110 = call i32 (ptr, ...) @printf(ptr nonnull @17, ptr %109)
  call void @_mp__String__void__dtor(ptr nonnull %16)
  call void @_mp__String__void__dtor(ptr nonnull %22)
  call void @_mp__String__void__dtor(ptr nonnull %s25)
  call void @_mp__String__void__dtor(ptr nonnull %26)
  call void @_mp__String__void__dtor(ptr nonnull %27)
  call void @_mp__String__void__dtor(ptr nonnull %29)
  call void @_mp__String__void__dtor(ptr nonnull %30)
  call void @_mp__String__void__dtor(ptr nonnull %31)
  call void @_mp__String__void__dtor(ptr nonnull %32)
  call void @_mp__String__void__dtor(ptr nonnull %33)
  call void @_mp__String__void__dtor(ptr nonnull %34)
  call void @_mp__String__void__dtor(ptr nonnull %35)
  call void @_mp__String__void__dtor(ptr nonnull %36)
  call void @_mp__String__void__dtor(ptr nonnull %37)
  call void @_mp__String__void__dtor(ptr nonnull %38)
  call void @_mp__String__void__dtor(ptr nonnull %39)
  call void @_mp__String__void__dtor(ptr nonnull %40)
  call void @_mp__String__void__dtor(ptr nonnull %0)
  call void @_mp__String__void__dtor(ptr nonnull %2)
  call void @_mp__String__void__dtor(ptr nonnull %s4)
  call void @_mp__String__void__dtor(ptr nonnull %s5)
  call void @_mp__String__void__dtor(ptr nonnull %s6)
  call void @_mp__String__void__dtor(ptr nonnull %s1)
  call void @_mp__String__void__dtor(ptr nonnull %4)
  call void @_mp__String__void__dtor(ptr nonnull %8)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__void__ctor__String_String(ptr, %_s__String__charptr_long_long, %_s__String__charptr_long_long) local_unnamed_addr

declare ptr @_mf__String__charptr__getRaw(ptr) local_unnamed_addr

declare void @_mp__String__void__ctor__String_string(ptr, %_s__String__charptr_long_long, ptr) local_unnamed_addr

declare void @_mp__String__void__ctor__String(ptr, %_s__String__charptr_long_long) local_unnamed_addr

declare void @_mp__String__void__opMul__short(ptr, i16) local_unnamed_addr

declare void @_mp__String__void__opMul__int(ptr, i32) local_unnamed_addr

declare void @_mp__String__void__ctor__char(ptr, i8) local_unnamed_addr

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr) local_unnamed_addr

declare i1 @_mf__String__bool__isEqual__String(ptr, %_s__String__charptr_long_long) local_unnamed_addr

declare void @_mp__String__void__append__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__void__append__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__void__dtor(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
