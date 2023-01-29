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
  %3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %5 = alloca %__String__charptr_long_long, align 8
  %6 = alloca %__String__charptr_long_long, align 8
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
  %21 = alloca %__String__charptr_long_long, align 8
  %22 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @anon.string.3) #2
  %.fca.0.load301 = load ptr, ptr %1, align 8
  %.fca.0.insert302 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load301, 0
  %.fca.1.gep303 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 1
  %.fca.1.load304 = load i64, ptr %.fca.1.gep303, align 8
  %.fca.1.insert305 = insertvalue %__String__charptr_long_long %.fca.0.insert302, i64 %.fca.1.load304, 1
  %.fca.2.gep306 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 2
  %.fca.2.load307 = load i64, ptr %.fca.2.gep306, align 8
  %.fca.2.insert308 = insertvalue %__String__charptr_long_long %.fca.1.insert305, i64 %.fca.2.load307, 2
  %.fca.0.load292 = load ptr, ptr %2, align 8
  %.fca.0.insert293 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load292, 0
  %.fca.1.gep294 = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 1
  %.fca.1.load295 = load i64, ptr %.fca.1.gep294, align 8
  %.fca.1.insert296 = insertvalue %__String__charptr_long_long %.fca.0.insert293, i64 %.fca.1.load295, 1
  %.fca.2.gep297 = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 2
  %.fca.2.load298 = load i64, ptr %.fca.2.gep297, align 8
  %.fca.2.insert299 = insertvalue %__String__charptr_long_long %.fca.1.insert296, i64 %.fca.2.load298, 2
  %23 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert308, %__String__charptr_long_long %.fca.2.insert299) #2
  %24 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %23)
  call void @_mp__String__void__ctor__string(ptr nonnull %3, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %4, ptr nonnull @anon.string.3) #2
  %.fca.0.load283 = load ptr, ptr %3, align 8
  %.fca.0.insert284 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load283, 0
  %.fca.1.gep285 = getelementptr inbounds %__String__charptr_long_long, ptr %3, i64 0, i32 1
  %.fca.1.load286 = load i64, ptr %.fca.1.gep285, align 8
  %.fca.1.insert287 = insertvalue %__String__charptr_long_long %.fca.0.insert284, i64 %.fca.1.load286, 1
  %.fca.2.gep288 = getelementptr inbounds %__String__charptr_long_long, ptr %3, i64 0, i32 2
  %.fca.2.load289 = load i64, ptr %.fca.2.gep288, align 8
  %.fca.2.insert290 = insertvalue %__String__charptr_long_long %.fca.1.insert287, i64 %.fca.2.load289, 2
  %.fca.0.load274 = load ptr, ptr %4, align 8
  %.fca.0.insert275 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load274, 0
  %.fca.1.gep276 = getelementptr inbounds %__String__charptr_long_long, ptr %4, i64 0, i32 1
  %.fca.1.load277 = load i64, ptr %.fca.1.gep276, align 8
  %.fca.1.insert278 = insertvalue %__String__charptr_long_long %.fca.0.insert275, i64 %.fca.1.load277, 1
  %.fca.2.gep279 = getelementptr inbounds %__String__charptr_long_long, ptr %4, i64 0, i32 2
  %.fca.2.load280 = load i64, ptr %.fca.2.gep279, align 8
  %.fca.2.insert281 = insertvalue %__String__charptr_long_long %.fca.1.insert278, i64 %.fca.2.load280, 2
  %25 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert290, %__String__charptr_long_long %.fca.2.insert281) #2
  %26 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %25)
  %27 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %25, ptr nonnull @anon.string.4) #2
  %28 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %27)
  call void @_mp__String__void__ctor__string(ptr nonnull %5, ptr nonnull @anon.string.5) #2
  %.fca.0.load173 = load ptr, ptr %5, align 8
  %.fca.0.insert174 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load173, 0
  %.fca.1.gep175 = getelementptr inbounds %__String__charptr_long_long, ptr %5, i64 0, i32 1
  %.fca.1.load176 = load i64, ptr %.fca.1.gep175, align 8
  %.fca.1.insert177 = insertvalue %__String__charptr_long_long %.fca.0.insert174, i64 %.fca.1.load176, 1
  %.fca.2.gep178 = getelementptr inbounds %__String__charptr_long_long, ptr %5, i64 0, i32 2
  %.fca.2.load179 = load i64, ptr %.fca.2.gep178, align 8
  %.fca.2.insert180 = insertvalue %__String__charptr_long_long %.fca.1.insert177, i64 %.fca.2.load179, 2
  %29 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert180, %__String__charptr_long_long %25) #2
  %30 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %29)
  %31 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %25, %__String__charptr_long_long %25) #2
  %32 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %31)
  %33 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %25, ptr nonnull @anon.string.6) #2
  %34 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %33, %__String__charptr_long_long %25) #2
  %35 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %34)
  call void @_mp__String__void__ctor__string(ptr nonnull %6, ptr nonnull @anon.string.7) #2
  %.fca.0.load164 = load ptr, ptr %6, align 8
  %.fca.0.insert165 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load164, 0
  %.fca.1.gep166 = getelementptr inbounds %__String__charptr_long_long, ptr %6, i64 0, i32 1
  %.fca.1.load167 = load i64, ptr %.fca.1.gep166, align 8
  %.fca.1.insert168 = insertvalue %__String__charptr_long_long %.fca.0.insert165, i64 %.fca.1.load167, 1
  %.fca.2.gep169 = getelementptr inbounds %__String__charptr_long_long, ptr %6, i64 0, i32 2
  %.fca.2.load170 = load i64, ptr %.fca.2.gep169, align 8
  %.fca.2.insert171 = insertvalue %__String__charptr_long_long %.fca.1.insert168, i64 %.fca.2.load170, 2
  %36 = call %__String__charptr_long_long @_f__void__String__op.plus__String_String(%__String__charptr_long_long %.fca.2.insert171, %__String__charptr_long_long %25) #2
  %37 = call %__String__charptr_long_long @_f__void__String__op.plus__String_string(%__String__charptr_long_long %36, ptr nonnull @anon.string.8) #2
  %38 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %37)
  call void @_mp__String__void__ctor__string(ptr nonnull %7, ptr nonnull @anon.string.9) #2
  %.fca.0.load155 = load ptr, ptr %7, align 8
  %.fca.0.insert156 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load155, 0
  %.fca.1.gep157 = getelementptr inbounds %__String__charptr_long_long, ptr %7, i64 0, i32 1
  %.fca.1.load158 = load i64, ptr %.fca.1.gep157, align 8
  %.fca.1.insert159 = insertvalue %__String__charptr_long_long %.fca.0.insert156, i64 %.fca.1.load158, 1
  %.fca.2.gep160 = getelementptr inbounds %__String__charptr_long_long, ptr %7, i64 0, i32 2
  %.fca.2.load161 = load i64, ptr %.fca.2.gep160, align 8
  %.fca.2.insert162 = insertvalue %__String__charptr_long_long %.fca.1.insert159, i64 %.fca.2.load161, 2
  %39 = call %__String__charptr_long_long @_f__void__String__op.mul__short_String(i16 4, %__String__charptr_long_long %.fca.2.insert162) #2
  %40 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %39)
  call void @_mp__String__void__ctor__string(ptr nonnull %8, ptr nonnull @anon.string.10) #2
  %.fca.0.load146 = load ptr, ptr %8, align 8
  %.fca.0.insert147 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load146, 0
  %.fca.1.gep148 = getelementptr inbounds %__String__charptr_long_long, ptr %8, i64 0, i32 1
  %.fca.1.load149 = load i64, ptr %.fca.1.gep148, align 8
  %.fca.1.insert150 = insertvalue %__String__charptr_long_long %.fca.0.insert147, i64 %.fca.1.load149, 1
  %.fca.2.gep151 = getelementptr inbounds %__String__charptr_long_long, ptr %8, i64 0, i32 2
  %.fca.2.load152 = load i64, ptr %.fca.2.gep151, align 8
  %.fca.2.insert153 = insertvalue %__String__charptr_long_long %.fca.1.insert150, i64 %.fca.2.load152, 2
  %41 = call %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long %.fca.2.insert153, i32 5) #2
  %42 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %41)
  call void @_mp__String__void__ctor__char(ptr nonnull %9, i8 97) #2
  %.fca.0.load122 = load ptr, ptr %9, align 8
  %.fca.0.insert123 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load122, 0
  %.fca.1.gep124 = getelementptr inbounds %__String__charptr_long_long, ptr %9, i64 0, i32 1
  %.fca.1.load125 = load i64, ptr %.fca.1.gep124, align 8
  %.fca.1.insert126 = insertvalue %__String__charptr_long_long %.fca.0.insert123, i64 %.fca.1.load125, 1
  %.fca.2.gep127 = getelementptr inbounds %__String__charptr_long_long, ptr %9, i64 0, i32 2
  %.fca.2.load128 = load i64, ptr %.fca.2.gep127, align 8
  %.fca.2.insert129 = insertvalue %__String__charptr_long_long %.fca.1.insert126, i64 %.fca.2.load128, 2
  %43 = call %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32 20, %__String__charptr_long_long %.fca.2.insert129) #2
  %44 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %43)
  call void @_mp__String__void__ctor__char(ptr nonnull %10, i8 99) #2
  %.fca.0.load113 = load ptr, ptr %10, align 8
  %.fca.0.insert114 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load113, 0
  %.fca.1.gep115 = getelementptr inbounds %__String__charptr_long_long, ptr %10, i64 0, i32 1
  %.fca.1.load116 = load i64, ptr %.fca.1.gep115, align 8
  %.fca.1.insert117 = insertvalue %__String__charptr_long_long %.fca.0.insert114, i64 %.fca.1.load116, 1
  %.fca.2.gep118 = getelementptr inbounds %__String__charptr_long_long, ptr %10, i64 0, i32 2
  %.fca.2.load119 = load i64, ptr %.fca.2.gep118, align 8
  %.fca.2.insert120 = insertvalue %__String__charptr_long_long %.fca.1.insert117, i64 %.fca.2.load119, 2
  %45 = call %__String__charptr_long_long @_f__void__String__op.mul__int_String(i32 2, %__String__charptr_long_long %.fca.2.insert120) #2
  %46 = call %__String__charptr_long_long @_f__void__String__op.mul__String_int(%__String__charptr_long_long %45, i32 7) #2
  %47 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, %__String__charptr_long_long %46)
  %48 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.29, ptr nonnull @anon.string.30) #2
  %49 = zext i1 %48 to i32
  %50 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %49)
  %51 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.34, ptr nonnull @anon.string.32) #2
  %52 = zext i1 %51 to i32
  %53 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %52)
  %54 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.34, ptr nonnull @anon.string.34) #2
  %55 = zext i1 %54 to i32
  %56 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %55)
  call void @_mp__String__void__ctor__string(ptr nonnull %11, ptr nonnull @anon.string.29) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %12, ptr nonnull @anon.string.30) #2
  %.fca.0.load92 = load ptr, ptr %11, align 8
  %.fca.0.insert93 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load92, 0
  %.fca.1.gep94 = getelementptr inbounds %__String__charptr_long_long, ptr %11, i64 0, i32 1
  %.fca.1.load95 = load i64, ptr %.fca.1.gep94, align 8
  %.fca.1.insert96 = insertvalue %__String__charptr_long_long %.fca.0.insert93, i64 %.fca.1.load95, 1
  %.fca.2.gep97 = getelementptr inbounds %__String__charptr_long_long, ptr %11, i64 0, i32 2
  %.fca.2.load98 = load i64, ptr %.fca.2.gep97, align 8
  %.fca.2.insert99 = insertvalue %__String__charptr_long_long %.fca.1.insert96, i64 %.fca.2.load98, 2
  %.fca.0.load83 = load ptr, ptr %12, align 8
  %.fca.0.insert84 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load83, 0
  %.fca.1.gep85 = getelementptr inbounds %__String__charptr_long_long, ptr %12, i64 0, i32 1
  %.fca.1.load86 = load i64, ptr %.fca.1.gep85, align 8
  %.fca.1.insert87 = insertvalue %__String__charptr_long_long %.fca.0.insert84, i64 %.fca.1.load86, 1
  %.fca.2.gep88 = getelementptr inbounds %__String__charptr_long_long, ptr %12, i64 0, i32 2
  %.fca.2.load89 = load i64, ptr %.fca.2.gep88, align 8
  %.fca.2.insert90 = insertvalue %__String__charptr_long_long %.fca.1.insert87, i64 %.fca.2.load89, 2
  %57 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %.fca.2.insert99, %__String__charptr_long_long %.fca.2.insert90) #2
  %58 = zext i1 %57 to i32
  %59 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %58)
  call void @_mp__String__void__ctor__string(ptr nonnull %13, ptr nonnull @anon.string.34) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %14, ptr nonnull @anon.string.32) #2
  %.fca.0.load74 = load ptr, ptr %13, align 8
  %.fca.0.insert75 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load74, 0
  %.fca.1.gep76 = getelementptr inbounds %__String__charptr_long_long, ptr %13, i64 0, i32 1
  %.fca.1.load77 = load i64, ptr %.fca.1.gep76, align 8
  %.fca.1.insert78 = insertvalue %__String__charptr_long_long %.fca.0.insert75, i64 %.fca.1.load77, 1
  %.fca.2.gep79 = getelementptr inbounds %__String__charptr_long_long, ptr %13, i64 0, i32 2
  %.fca.2.load80 = load i64, ptr %.fca.2.gep79, align 8
  %.fca.2.insert81 = insertvalue %__String__charptr_long_long %.fca.1.insert78, i64 %.fca.2.load80, 2
  %.fca.0.load65 = load ptr, ptr %14, align 8
  %.fca.0.insert66 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load65, 0
  %.fca.1.gep67 = getelementptr inbounds %__String__charptr_long_long, ptr %14, i64 0, i32 1
  %.fca.1.load68 = load i64, ptr %.fca.1.gep67, align 8
  %.fca.1.insert69 = insertvalue %__String__charptr_long_long %.fca.0.insert66, i64 %.fca.1.load68, 1
  %.fca.2.gep70 = getelementptr inbounds %__String__charptr_long_long, ptr %14, i64 0, i32 2
  %.fca.2.load71 = load i64, ptr %.fca.2.gep70, align 8
  %.fca.2.insert72 = insertvalue %__String__charptr_long_long %.fca.1.insert69, i64 %.fca.2.load71, 2
  %60 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %.fca.2.insert81, %__String__charptr_long_long %.fca.2.insert72) #2
  %61 = zext i1 %60 to i32
  %62 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %61)
  call void @_mp__String__void__ctor__string(ptr nonnull %15, ptr nonnull @anon.string.34) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %16, ptr nonnull @anon.string.34) #2
  %.fca.0.load56 = load ptr, ptr %15, align 8
  %.fca.0.insert57 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load56, 0
  %.fca.1.gep58 = getelementptr inbounds %__String__charptr_long_long, ptr %15, i64 0, i32 1
  %.fca.1.load59 = load i64, ptr %.fca.1.gep58, align 8
  %.fca.1.insert60 = insertvalue %__String__charptr_long_long %.fca.0.insert57, i64 %.fca.1.load59, 1
  %.fca.2.gep61 = getelementptr inbounds %__String__charptr_long_long, ptr %15, i64 0, i32 2
  %.fca.2.load62 = load i64, ptr %.fca.2.gep61, align 8
  %.fca.2.insert63 = insertvalue %__String__charptr_long_long %.fca.1.insert60, i64 %.fca.2.load62, 2
  %.fca.0.load47 = load ptr, ptr %16, align 8
  %.fca.0.insert48 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load47, 0
  %.fca.1.gep49 = getelementptr inbounds %__String__charptr_long_long, ptr %16, i64 0, i32 1
  %.fca.1.load50 = load i64, ptr %.fca.1.gep49, align 8
  %.fca.1.insert51 = insertvalue %__String__charptr_long_long %.fca.0.insert48, i64 %.fca.1.load50, 1
  %.fca.2.gep52 = getelementptr inbounds %__String__charptr_long_long, ptr %16, i64 0, i32 2
  %.fca.2.load53 = load i64, ptr %.fca.2.gep52, align 8
  %.fca.2.insert54 = insertvalue %__String__charptr_long_long %.fca.1.insert51, i64 %.fca.2.load53, 2
  %63 = call i1 @_f__void__bool__op.equal__String_String(%__String__charptr_long_long %.fca.2.insert63, %__String__charptr_long_long %.fca.2.insert54) #2
  %64 = zext i1 %63 to i32
  %65 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %64)
  %66 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.29, ptr nonnull @anon.string.30) #2
  %67 = xor i1 %66, true
  %68 = zext i1 %67 to i32
  %69 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %68)
  %70 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.34, ptr nonnull @anon.string.32) #2
  %71 = xor i1 %70, true
  %72 = zext i1 %71 to i32
  %73 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %72)
  %74 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.34, ptr nonnull @anon.string.34) #2
  %75 = xor i1 %74, true
  %76 = zext i1 %75 to i32
  %77 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %76)
  call void @_mp__String__void__ctor__string(ptr nonnull %17, ptr nonnull @anon.string.29) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %18, ptr nonnull @anon.string.30) #2
  %.fca.0.load38 = load ptr, ptr %17, align 8
  %.fca.0.insert39 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load38, 0
  %.fca.1.gep40 = getelementptr inbounds %__String__charptr_long_long, ptr %17, i64 0, i32 1
  %.fca.1.load41 = load i64, ptr %.fca.1.gep40, align 8
  %.fca.1.insert42 = insertvalue %__String__charptr_long_long %.fca.0.insert39, i64 %.fca.1.load41, 1
  %.fca.2.gep43 = getelementptr inbounds %__String__charptr_long_long, ptr %17, i64 0, i32 2
  %.fca.2.load44 = load i64, ptr %.fca.2.gep43, align 8
  %.fca.2.insert45 = insertvalue %__String__charptr_long_long %.fca.1.insert42, i64 %.fca.2.load44, 2
  %.fca.0.load29 = load ptr, ptr %18, align 8
  %.fca.0.insert30 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load29, 0
  %.fca.1.gep31 = getelementptr inbounds %__String__charptr_long_long, ptr %18, i64 0, i32 1
  %.fca.1.load32 = load i64, ptr %.fca.1.gep31, align 8
  %.fca.1.insert33 = insertvalue %__String__charptr_long_long %.fca.0.insert30, i64 %.fca.1.load32, 1
  %.fca.2.gep34 = getelementptr inbounds %__String__charptr_long_long, ptr %18, i64 0, i32 2
  %.fca.2.load35 = load i64, ptr %.fca.2.gep34, align 8
  %.fca.2.insert36 = insertvalue %__String__charptr_long_long %.fca.1.insert33, i64 %.fca.2.load35, 2
  %78 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %.fca.2.insert45, %__String__charptr_long_long %.fca.2.insert36) #2
  %79 = zext i1 %78 to i32
  %80 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %79)
  call void @_mp__String__void__ctor__string(ptr nonnull %19, ptr nonnull @anon.string.34) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %20, ptr nonnull @anon.string.32) #2
  %.fca.0.load20 = load ptr, ptr %19, align 8
  %.fca.0.insert21 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load20, 0
  %.fca.1.gep22 = getelementptr inbounds %__String__charptr_long_long, ptr %19, i64 0, i32 1
  %.fca.1.load23 = load i64, ptr %.fca.1.gep22, align 8
  %.fca.1.insert24 = insertvalue %__String__charptr_long_long %.fca.0.insert21, i64 %.fca.1.load23, 1
  %.fca.2.gep25 = getelementptr inbounds %__String__charptr_long_long, ptr %19, i64 0, i32 2
  %.fca.2.load26 = load i64, ptr %.fca.2.gep25, align 8
  %.fca.2.insert27 = insertvalue %__String__charptr_long_long %.fca.1.insert24, i64 %.fca.2.load26, 2
  %.fca.0.load11 = load ptr, ptr %20, align 8
  %.fca.0.insert12 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load11, 0
  %.fca.1.gep13 = getelementptr inbounds %__String__charptr_long_long, ptr %20, i64 0, i32 1
  %.fca.1.load14 = load i64, ptr %.fca.1.gep13, align 8
  %.fca.1.insert15 = insertvalue %__String__charptr_long_long %.fca.0.insert12, i64 %.fca.1.load14, 1
  %.fca.2.gep16 = getelementptr inbounds %__String__charptr_long_long, ptr %20, i64 0, i32 2
  %.fca.2.load17 = load i64, ptr %.fca.2.gep16, align 8
  %.fca.2.insert18 = insertvalue %__String__charptr_long_long %.fca.1.insert15, i64 %.fca.2.load17, 2
  %81 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %.fca.2.insert27, %__String__charptr_long_long %.fca.2.insert18) #2
  %82 = zext i1 %81 to i32
  %83 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %82)
  call void @_mp__String__void__ctor__string(ptr nonnull %21, ptr nonnull @anon.string.34) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %22, ptr nonnull @anon.string.34) #2
  %.fca.0.load2 = load ptr, ptr %21, align 8
  %.fca.0.insert3 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load2, 0
  %.fca.1.gep4 = getelementptr inbounds %__String__charptr_long_long, ptr %21, i64 0, i32 1
  %.fca.1.load5 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert6 = insertvalue %__String__charptr_long_long %.fca.0.insert3, i64 %.fca.1.load5, 1
  %.fca.2.gep7 = getelementptr inbounds %__String__charptr_long_long, ptr %21, i64 0, i32 2
  %.fca.2.load8 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert9 = insertvalue %__String__charptr_long_long %.fca.1.insert6, i64 %.fca.2.load8, 2
  %.fca.0.load = load ptr, ptr %22, align 8
  %.fca.0.insert = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %22, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %__String__charptr_long_long %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %22, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %__String__charptr_long_long %.fca.1.insert, i64 %.fca.2.load, 2
  %84 = call i1 @_f__void__bool__op.notequal__String_String(%__String__charptr_long_long %.fca.2.insert9, %__String__charptr_long_long %.fca.2.insert) #2
  %85 = zext i1 %84 to i32
  %86 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %85)
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
