; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.3 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.4 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@anon.string.5 = private unnamed_addr constant [5 x i8] c"Hi! \00", align 1
@anon.string.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@anon.string.7 = private unnamed_addr constant [8 x i8] c"Prefix \00", align 1
@anon.string.8 = private unnamed_addr constant [8 x i8] c" Suffix\00", align 1
@anon.string.9 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@anon.string.10 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@printf.str.10 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.13 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@printf.str.16 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@printf.str.19 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.29 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.30 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@anon.string.32 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.22 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.34 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %s1 = alloca %__String__charptr_long_long, align 8
  %3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %5 = alloca %__String__charptr_long_long, align 8
  %6 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %7 = alloca %__String__charptr_long_long, align 8
  %8 = alloca %__String__charptr_long_long, align 8
  %9 = alloca %__String__charptr_long_long, align 8
  %10 = alloca %__String__charptr_long_long, align 8
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
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @anon.string.3) #2
  %.fca.0.load293 = load ptr, ptr %1, align 8
  %.fca.0.insert294 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load293, 0
  %.fca.1.gep295 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 1
  %.fca.1.load296 = load i64, ptr %.fca.1.gep295, align 8
  %.fca.1.insert297 = insertvalue %__String__charptr_long_long %.fca.0.insert294, i64 %.fca.1.load296, 1
  %.fca.2.gep298 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 2
  %.fca.2.load299 = load i64, ptr %.fca.2.gep298, align 8
  %.fca.2.insert300 = insertvalue %__String__charptr_long_long %.fca.1.insert297, i64 %.fca.2.load299, 2
  %.fca.0.load284 = load ptr, ptr %2, align 8
  %.fca.0.insert285 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load284, 0
  %.fca.1.gep286 = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 1
  %.fca.1.load287 = load i64, ptr %.fca.1.gep286, align 8
  %.fca.1.insert288 = insertvalue %__String__charptr_long_long %.fca.0.insert285, i64 %.fca.1.load287, 1
  %.fca.2.gep289 = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 2
  %.fca.2.load290 = load i64, ptr %.fca.2.gep289, align 8
  %.fca.2.insert291 = insertvalue %__String__charptr_long_long %.fca.1.insert288, i64 %.fca.2.load290, 2
  %21 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert300, %__String__charptr_long_long %.fca.2.insert291) #2
  %22 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %21)
  call void @_mp__String__void__ctor__string(ptr nonnull %s1, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %3, ptr nonnull @anon.string.3) #2
  %.fca.0.load197 = load ptr, ptr %s1, align 8
  %.fca.0.insert198 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load197, 0
  %.fca.1.gep199 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 1
  %.fca.1.load200 = load i64, ptr %.fca.1.gep199, align 8
  %.fca.1.insert201 = insertvalue %__String__charptr_long_long %.fca.0.insert198, i64 %.fca.1.load200, 1
  %.fca.2.gep202 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 2
  %.fca.2.load203 = load i64, ptr %.fca.2.gep202, align 8
  %.fca.2.insert204 = insertvalue %__String__charptr_long_long %.fca.1.insert201, i64 %.fca.2.load203, 2
  %.fca.0.load188 = load ptr, ptr %3, align 8
  %.fca.0.insert189 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load188, 0
  %.fca.1.gep190 = getelementptr inbounds %__String__charptr_long_long, ptr %3, i64 0, i32 1
  %.fca.1.load191 = load i64, ptr %.fca.1.gep190, align 8
  %.fca.1.insert192 = insertvalue %__String__charptr_long_long %.fca.0.insert189, i64 %.fca.1.load191, 1
  %.fca.2.gep193 = getelementptr inbounds %__String__charptr_long_long, ptr %3, i64 0, i32 2
  %.fca.2.load194 = load i64, ptr %.fca.2.gep193, align 8
  %.fca.2.insert195 = insertvalue %__String__charptr_long_long %.fca.1.insert192, i64 %.fca.2.load194, 2
  %23 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert204, %__String__charptr_long_long %.fca.2.insert195) #2
  %.fca.0.extract205 = extractvalue %__String__charptr_long_long %23, 0
  store ptr %.fca.0.extract205, ptr %s1, align 8
  %.fca.1.extract207 = extractvalue %__String__charptr_long_long %23, 1
  store i64 %.fca.1.extract207, ptr %.fca.1.gep199, align 8
  %.fca.2.extract209 = extractvalue %__String__charptr_long_long %23, 2
  store i64 %.fca.2.extract209, ptr %.fca.2.gep202, align 8
  %24 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %23)
  %.fca.0.load221 = load ptr, ptr %s1, align 8
  %.fca.0.insert222 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load221, 0
  %.fca.1.load224 = load i64, ptr %.fca.1.gep199, align 8
  %.fca.1.insert225 = insertvalue %__String__charptr_long_long %.fca.0.insert222, i64 %.fca.1.load224, 1
  %.fca.2.load227 = load i64, ptr %.fca.2.gep202, align 8
  %.fca.2.insert228 = insertvalue %__String__charptr_long_long %.fca.1.insert225, i64 %.fca.2.load227, 2
  %25 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %.fca.2.insert228, ptr nonnull @anon.string.4) #2
  %26 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %25)
  call void @_mp__String__void__ctor__string(ptr nonnull %4, ptr nonnull @anon.string.5) #2
  %.fca.0.load179 = load ptr, ptr %4, align 8
  %.fca.0.insert180 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load179, 0
  %.fca.1.gep181 = getelementptr inbounds %__String__charptr_long_long, ptr %4, i64 0, i32 1
  %.fca.1.load182 = load i64, ptr %.fca.1.gep181, align 8
  %.fca.1.insert183 = insertvalue %__String__charptr_long_long %.fca.0.insert180, i64 %.fca.1.load182, 1
  %.fca.2.gep184 = getelementptr inbounds %__String__charptr_long_long, ptr %4, i64 0, i32 2
  %.fca.2.load185 = load i64, ptr %.fca.2.gep184, align 8
  %.fca.2.insert186 = insertvalue %__String__charptr_long_long %.fca.1.insert183, i64 %.fca.2.load185, 2
  %.fca.0.load230 = load ptr, ptr %s1, align 8
  %.fca.0.insert231 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load230, 0
  %.fca.1.load233 = load i64, ptr %.fca.1.gep199, align 8
  %.fca.1.insert234 = insertvalue %__String__charptr_long_long %.fca.0.insert231, i64 %.fca.1.load233, 1
  %.fca.2.load236 = load i64, ptr %.fca.2.gep202, align 8
  %.fca.2.insert237 = insertvalue %__String__charptr_long_long %.fca.1.insert234, i64 %.fca.2.load236, 2
  %27 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert186, %__String__charptr_long_long %.fca.2.insert237) #2
  %28 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %27)
  %.fca.0.load239 = load ptr, ptr %s1, align 8
  %.fca.0.insert240 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load239, 0
  %.fca.1.load242 = load i64, ptr %.fca.1.gep199, align 8
  %.fca.1.insert243 = insertvalue %__String__charptr_long_long %.fca.0.insert240, i64 %.fca.1.load242, 1
  %.fca.2.load245 = load i64, ptr %.fca.2.gep202, align 8
  %.fca.2.insert246 = insertvalue %__String__charptr_long_long %.fca.1.insert243, i64 %.fca.2.load245, 2
  %29 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert246, %__String__charptr_long_long %.fca.2.insert246) #2
  %30 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %29)
  %.fca.0.load257 = load ptr, ptr %s1, align 8
  %.fca.0.insert258 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load257, 0
  %.fca.1.load260 = load i64, ptr %.fca.1.gep199, align 8
  %.fca.1.insert261 = insertvalue %__String__charptr_long_long %.fca.0.insert258, i64 %.fca.1.load260, 1
  %.fca.2.load263 = load i64, ptr %.fca.2.gep202, align 8
  %.fca.2.insert264 = insertvalue %__String__charptr_long_long %.fca.1.insert261, i64 %.fca.2.load263, 2
  %31 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %.fca.2.insert264, ptr nonnull @anon.string.6) #2
  %.fca.0.load266 = load ptr, ptr %s1, align 8
  %.fca.0.insert267 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load266, 0
  %.fca.1.load269 = load i64, ptr %.fca.1.gep199, align 8
  %.fca.1.insert270 = insertvalue %__String__charptr_long_long %.fca.0.insert267, i64 %.fca.1.load269, 1
  %.fca.2.load272 = load i64, ptr %.fca.2.gep202, align 8
  %.fca.2.insert273 = insertvalue %__String__charptr_long_long %.fca.1.insert270, i64 %.fca.2.load272, 2
  %32 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %31, %__String__charptr_long_long %.fca.2.insert273) #2
  %33 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %32)
  call void @_mp__String__void__ctor__string(ptr nonnull %5, ptr nonnull @anon.string.7) #2
  %.fca.0.load170 = load ptr, ptr %5, align 8
  %.fca.0.insert171 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load170, 0
  %.fca.1.gep172 = getelementptr inbounds %__String__charptr_long_long, ptr %5, i64 0, i32 1
  %.fca.1.load173 = load i64, ptr %.fca.1.gep172, align 8
  %.fca.1.insert174 = insertvalue %__String__charptr_long_long %.fca.0.insert171, i64 %.fca.1.load173, 1
  %.fca.2.gep175 = getelementptr inbounds %__String__charptr_long_long, ptr %5, i64 0, i32 2
  %.fca.2.load176 = load i64, ptr %.fca.2.gep175, align 8
  %.fca.2.insert177 = insertvalue %__String__charptr_long_long %.fca.1.insert174, i64 %.fca.2.load176, 2
  %.fca.0.load275 = load ptr, ptr %s1, align 8
  %.fca.0.insert276 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load275, 0
  %.fca.1.load278 = load i64, ptr %.fca.1.gep199, align 8
  %.fca.1.insert279 = insertvalue %__String__charptr_long_long %.fca.0.insert276, i64 %.fca.1.load278, 1
  %.fca.2.load281 = load i64, ptr %.fca.2.gep202, align 8
  %.fca.2.insert282 = insertvalue %__String__charptr_long_long %.fca.1.insert279, i64 %.fca.2.load281, 2
  %34 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert177, %__String__charptr_long_long %.fca.2.insert282) #2
  %35 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %34, ptr nonnull @anon.string.8) #2
  %36 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %35)
  call void @_mp__String__void__ctor__string(ptr nonnull %6, ptr nonnull @anon.string.9) #2
  %.fca.0.load161 = load ptr, ptr %6, align 8
  %.fca.0.insert162 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load161, 0
  %.fca.1.gep163 = getelementptr inbounds %__String__charptr_long_long, ptr %6, i64 0, i32 1
  %.fca.1.load164 = load i64, ptr %.fca.1.gep163, align 8
  %.fca.1.insert165 = insertvalue %__String__charptr_long_long %.fca.0.insert162, i64 %.fca.1.load164, 1
  %.fca.2.gep166 = getelementptr inbounds %__String__charptr_long_long, ptr %6, i64 0, i32 2
  %.fca.2.load167 = load i64, ptr %.fca.2.gep166, align 8
  %.fca.2.insert168 = insertvalue %__String__charptr_long_long %.fca.1.insert165, i64 %.fca.2.load167, 2
  %37 = call %__String__charptr_long_long @_f__void__String__op.mul__short_String(i16 4, %__String__charptr_long_long %.fca.2.insert168) #2
  %38 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %37)
  call void @_mp__String__void__ctor__string(ptr nonnull %s2, ptr nonnull @anon.string.10) #2
  %.fca.0.load137 = load ptr, ptr %s2, align 8
  %.fca.0.insert138 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load137, 0
  %.fca.1.gep139 = getelementptr inbounds %__String__charptr_long_long, ptr %s2, i64 0, i32 1
  %.fca.1.load140 = load i64, ptr %.fca.1.gep139, align 8
  %.fca.1.insert141 = insertvalue %__String__charptr_long_long %.fca.0.insert138, i64 %.fca.1.load140, 1
  %.fca.2.gep142 = getelementptr inbounds %__String__charptr_long_long, ptr %s2, i64 0, i32 2
  %.fca.2.load143 = load i64, ptr %.fca.2.gep142, align 8
  %.fca.2.insert144 = insertvalue %__String__charptr_long_long %.fca.1.insert141, i64 %.fca.2.load143, 2
  %39 = call %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long %.fca.2.insert144, i32 5) #2
  %.fca.0.extract145 = extractvalue %__String__charptr_long_long %39, 0
  store ptr %.fca.0.extract145, ptr %s2, align 8
  %.fca.1.extract147 = extractvalue %__String__charptr_long_long %39, 1
  store i64 %.fca.1.extract147, ptr %.fca.1.gep139, align 8
  %.fca.2.extract149 = extractvalue %__String__charptr_long_long %39, 2
  store i64 %.fca.2.extract149, ptr %.fca.2.gep142, align 8
  %40 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %39)
  call void @_mp__String__void__ctor__char(ptr nonnull %7, i8 97) #2
  %.fca.0.load128 = load ptr, ptr %7, align 8
  %.fca.0.insert129 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load128, 0
  %.fca.1.gep130 = getelementptr inbounds %__String__charptr_long_long, ptr %7, i64 0, i32 1
  %.fca.1.load131 = load i64, ptr %.fca.1.gep130, align 8
  %.fca.1.insert132 = insertvalue %__String__charptr_long_long %.fca.0.insert129, i64 %.fca.1.load131, 1
  %.fca.2.gep133 = getelementptr inbounds %__String__charptr_long_long, ptr %7, i64 0, i32 2
  %.fca.2.load134 = load i64, ptr %.fca.2.gep133, align 8
  %.fca.2.insert135 = insertvalue %__String__charptr_long_long %.fca.1.insert132, i64 %.fca.2.load134, 2
  %41 = call %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32 20, %__String__charptr_long_long %.fca.2.insert135) #2
  %42 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %41)
  call void @_mp__String__void__ctor__char(ptr nonnull %8, i8 99) #2
  %.fca.0.load119 = load ptr, ptr %8, align 8
  %.fca.0.insert120 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load119, 0
  %.fca.1.gep121 = getelementptr inbounds %__String__charptr_long_long, ptr %8, i64 0, i32 1
  %.fca.1.load122 = load i64, ptr %.fca.1.gep121, align 8
  %.fca.1.insert123 = insertvalue %__String__charptr_long_long %.fca.0.insert120, i64 %.fca.1.load122, 1
  %.fca.2.gep124 = getelementptr inbounds %__String__charptr_long_long, ptr %8, i64 0, i32 2
  %.fca.2.load125 = load i64, ptr %.fca.2.gep124, align 8
  %.fca.2.insert126 = insertvalue %__String__charptr_long_long %.fca.1.insert123, i64 %.fca.2.load125, 2
  %43 = call %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32 2, %__String__charptr_long_long %.fca.2.insert126) #2
  %44 = call %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long %43, i32 7) #2
  %45 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %44)
  %46 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.29, ptr nonnull @anon.string.30) #2
  %47 = zext i1 %46 to i32
  %48 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %47)
  %49 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.34, ptr nonnull @anon.string.32) #2
  %50 = zext i1 %49 to i32
  %51 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %50)
  %52 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.34, ptr nonnull @anon.string.34) #2
  %53 = zext i1 %52 to i32
  %54 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %53)
  call void @_mp__String__void__ctor__string(ptr nonnull %9, ptr nonnull @anon.string.29) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %10, ptr nonnull @anon.string.30) #2
  %.fca.0.load92 = load ptr, ptr %9, align 8
  %.fca.0.insert93 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load92, 0
  %.fca.1.gep94 = getelementptr inbounds %__String__charptr_long_long, ptr %9, i64 0, i32 1
  %.fca.1.load95 = load i64, ptr %.fca.1.gep94, align 8
  %.fca.1.insert96 = insertvalue %__String__charptr_long_long %.fca.0.insert93, i64 %.fca.1.load95, 1
  %.fca.2.gep97 = getelementptr inbounds %__String__charptr_long_long, ptr %9, i64 0, i32 2
  %.fca.2.load98 = load i64, ptr %.fca.2.gep97, align 8
  %.fca.2.insert99 = insertvalue %__String__charptr_long_long %.fca.1.insert96, i64 %.fca.2.load98, 2
  %.fca.0.load83 = load ptr, ptr %10, align 8
  %.fca.0.insert84 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load83, 0
  %.fca.1.gep85 = getelementptr inbounds %__String__charptr_long_long, ptr %10, i64 0, i32 1
  %.fca.1.load86 = load i64, ptr %.fca.1.gep85, align 8
  %.fca.1.insert87 = insertvalue %__String__charptr_long_long %.fca.0.insert84, i64 %.fca.1.load86, 1
  %.fca.2.gep88 = getelementptr inbounds %__String__charptr_long_long, ptr %10, i64 0, i32 2
  %.fca.2.load89 = load i64, ptr %.fca.2.gep88, align 8
  %.fca.2.insert90 = insertvalue %__String__charptr_long_long %.fca.1.insert87, i64 %.fca.2.load89, 2
  %55 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %.fca.2.insert99, %__String__charptr_long_long %.fca.2.insert90) #2
  %56 = zext i1 %55 to i32
  %57 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %56)
  call void @_mp__String__void__ctor__string(ptr nonnull %11, ptr nonnull @anon.string.34) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %12, ptr nonnull @anon.string.32) #2
  %.fca.0.load74 = load ptr, ptr %11, align 8
  %.fca.0.insert75 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load74, 0
  %.fca.1.gep76 = getelementptr inbounds %__String__charptr_long_long, ptr %11, i64 0, i32 1
  %.fca.1.load77 = load i64, ptr %.fca.1.gep76, align 8
  %.fca.1.insert78 = insertvalue %__String__charptr_long_long %.fca.0.insert75, i64 %.fca.1.load77, 1
  %.fca.2.gep79 = getelementptr inbounds %__String__charptr_long_long, ptr %11, i64 0, i32 2
  %.fca.2.load80 = load i64, ptr %.fca.2.gep79, align 8
  %.fca.2.insert81 = insertvalue %__String__charptr_long_long %.fca.1.insert78, i64 %.fca.2.load80, 2
  %.fca.0.load65 = load ptr, ptr %12, align 8
  %.fca.0.insert66 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load65, 0
  %.fca.1.gep67 = getelementptr inbounds %__String__charptr_long_long, ptr %12, i64 0, i32 1
  %.fca.1.load68 = load i64, ptr %.fca.1.gep67, align 8
  %.fca.1.insert69 = insertvalue %__String__charptr_long_long %.fca.0.insert66, i64 %.fca.1.load68, 1
  %.fca.2.gep70 = getelementptr inbounds %__String__charptr_long_long, ptr %12, i64 0, i32 2
  %.fca.2.load71 = load i64, ptr %.fca.2.gep70, align 8
  %.fca.2.insert72 = insertvalue %__String__charptr_long_long %.fca.1.insert69, i64 %.fca.2.load71, 2
  %58 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %.fca.2.insert81, %__String__charptr_long_long %.fca.2.insert72) #2
  %59 = zext i1 %58 to i32
  %60 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %59)
  call void @_mp__String__void__ctor__string(ptr nonnull %13, ptr nonnull @anon.string.34) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %14, ptr nonnull @anon.string.34) #2
  %.fca.0.load56 = load ptr, ptr %13, align 8
  %.fca.0.insert57 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load56, 0
  %.fca.1.gep58 = getelementptr inbounds %__String__charptr_long_long, ptr %13, i64 0, i32 1
  %.fca.1.load59 = load i64, ptr %.fca.1.gep58, align 8
  %.fca.1.insert60 = insertvalue %__String__charptr_long_long %.fca.0.insert57, i64 %.fca.1.load59, 1
  %.fca.2.gep61 = getelementptr inbounds %__String__charptr_long_long, ptr %13, i64 0, i32 2
  %.fca.2.load62 = load i64, ptr %.fca.2.gep61, align 8
  %.fca.2.insert63 = insertvalue %__String__charptr_long_long %.fca.1.insert60, i64 %.fca.2.load62, 2
  %.fca.0.load47 = load ptr, ptr %14, align 8
  %.fca.0.insert48 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load47, 0
  %.fca.1.gep49 = getelementptr inbounds %__String__charptr_long_long, ptr %14, i64 0, i32 1
  %.fca.1.load50 = load i64, ptr %.fca.1.gep49, align 8
  %.fca.1.insert51 = insertvalue %__String__charptr_long_long %.fca.0.insert48, i64 %.fca.1.load50, 1
  %.fca.2.gep52 = getelementptr inbounds %__String__charptr_long_long, ptr %14, i64 0, i32 2
  %.fca.2.load53 = load i64, ptr %.fca.2.gep52, align 8
  %.fca.2.insert54 = insertvalue %__String__charptr_long_long %.fca.1.insert51, i64 %.fca.2.load53, 2
  %61 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %.fca.2.insert63, %__String__charptr_long_long %.fca.2.insert54) #2
  %62 = zext i1 %61 to i32
  %63 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %62)
  %64 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.29, ptr nonnull @anon.string.30) #2
  %65 = xor i1 %64, true
  %66 = zext i1 %65 to i32
  %67 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %66)
  %68 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.34, ptr nonnull @anon.string.32) #2
  %69 = xor i1 %68, true
  %70 = zext i1 %69 to i32
  %71 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %70)
  %72 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.34, ptr nonnull @anon.string.34) #2
  %73 = xor i1 %72, true
  %74 = zext i1 %73 to i32
  %75 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %74)
  call void @_mp__String__void__ctor__string(ptr nonnull %15, ptr nonnull @anon.string.29) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %16, ptr nonnull @anon.string.30) #2
  %.fca.0.load38 = load ptr, ptr %15, align 8
  %.fca.0.insert39 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load38, 0
  %.fca.1.gep40 = getelementptr inbounds %__String__charptr_long_long, ptr %15, i64 0, i32 1
  %.fca.1.load41 = load i64, ptr %.fca.1.gep40, align 8
  %.fca.1.insert42 = insertvalue %__String__charptr_long_long %.fca.0.insert39, i64 %.fca.1.load41, 1
  %.fca.2.gep43 = getelementptr inbounds %__String__charptr_long_long, ptr %15, i64 0, i32 2
  %.fca.2.load44 = load i64, ptr %.fca.2.gep43, align 8
  %.fca.2.insert45 = insertvalue %__String__charptr_long_long %.fca.1.insert42, i64 %.fca.2.load44, 2
  %.fca.0.load29 = load ptr, ptr %16, align 8
  %.fca.0.insert30 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load29, 0
  %.fca.1.gep31 = getelementptr inbounds %__String__charptr_long_long, ptr %16, i64 0, i32 1
  %.fca.1.load32 = load i64, ptr %.fca.1.gep31, align 8
  %.fca.1.insert33 = insertvalue %__String__charptr_long_long %.fca.0.insert30, i64 %.fca.1.load32, 1
  %.fca.2.gep34 = getelementptr inbounds %__String__charptr_long_long, ptr %16, i64 0, i32 2
  %.fca.2.load35 = load i64, ptr %.fca.2.gep34, align 8
  %.fca.2.insert36 = insertvalue %__String__charptr_long_long %.fca.1.insert33, i64 %.fca.2.load35, 2
  %76 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %.fca.2.insert45, %__String__charptr_long_long %.fca.2.insert36) #2
  %77 = zext i1 %76 to i32
  %78 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %77)
  call void @_mp__String__void__ctor__string(ptr nonnull %17, ptr nonnull @anon.string.34) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %18, ptr nonnull @anon.string.32) #2
  %.fca.0.load20 = load ptr, ptr %17, align 8
  %.fca.0.insert21 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load20, 0
  %.fca.1.gep22 = getelementptr inbounds %__String__charptr_long_long, ptr %17, i64 0, i32 1
  %.fca.1.load23 = load i64, ptr %.fca.1.gep22, align 8
  %.fca.1.insert24 = insertvalue %__String__charptr_long_long %.fca.0.insert21, i64 %.fca.1.load23, 1
  %.fca.2.gep25 = getelementptr inbounds %__String__charptr_long_long, ptr %17, i64 0, i32 2
  %.fca.2.load26 = load i64, ptr %.fca.2.gep25, align 8
  %.fca.2.insert27 = insertvalue %__String__charptr_long_long %.fca.1.insert24, i64 %.fca.2.load26, 2
  %.fca.0.load11 = load ptr, ptr %18, align 8
  %.fca.0.insert12 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load11, 0
  %.fca.1.gep13 = getelementptr inbounds %__String__charptr_long_long, ptr %18, i64 0, i32 1
  %.fca.1.load14 = load i64, ptr %.fca.1.gep13, align 8
  %.fca.1.insert15 = insertvalue %__String__charptr_long_long %.fca.0.insert12, i64 %.fca.1.load14, 1
  %.fca.2.gep16 = getelementptr inbounds %__String__charptr_long_long, ptr %18, i64 0, i32 2
  %.fca.2.load17 = load i64, ptr %.fca.2.gep16, align 8
  %.fca.2.insert18 = insertvalue %__String__charptr_long_long %.fca.1.insert15, i64 %.fca.2.load17, 2
  %79 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %.fca.2.insert27, %__String__charptr_long_long %.fca.2.insert18) #2
  %80 = zext i1 %79 to i32
  %81 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %80)
  call void @_mp__String__void__ctor__string(ptr nonnull %19, ptr nonnull @anon.string.34) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %20, ptr nonnull @anon.string.34) #2
  %.fca.0.load2 = load ptr, ptr %19, align 8
  %.fca.0.insert3 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load2, 0
  %.fca.1.gep4 = getelementptr inbounds %__String__charptr_long_long, ptr %19, i64 0, i32 1
  %.fca.1.load5 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert6 = insertvalue %__String__charptr_long_long %.fca.0.insert3, i64 %.fca.1.load5, 1
  %.fca.2.gep7 = getelementptr inbounds %__String__charptr_long_long, ptr %19, i64 0, i32 2
  %.fca.2.load8 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert9 = insertvalue %__String__charptr_long_long %.fca.1.insert6, i64 %.fca.2.load8, 2
  %.fca.0.load = load ptr, ptr %20, align 8
  %.fca.0.insert = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %20, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %__String__charptr_long_long %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %20, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %__String__charptr_long_long %.fca.1.insert, i64 %.fca.2.load, 2
  %82 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %.fca.2.insert9, %__String__charptr_long_long %.fca.2.insert) #2
  %83 = zext i1 %82 to i32
  %84 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %83)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long, %__String__charptr_long_long) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__short_String(i16, %__String__charptr_long_long) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long, i32) local_unnamed_addr

declare void @_mp__String__void__ctor__char(ptr, i8) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32, %__String__charptr_long_long) local_unnamed_addr

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr) local_unnamed_addr

declare i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long, %__String__charptr_long_long) local_unnamed_addr

declare i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long, %__String__charptr_long_long) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
