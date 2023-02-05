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
@anon.string.10 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@printf.str.13 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@printf.str.16 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@printf.str.19 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.29 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.30 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@anon.string.32 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.22 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.35 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.37 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@anon.string.38 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@printf.str.25 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
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
  %14 = alloca %__String__charptr_long_long, align 8
  %15 = alloca %__String__charptr_long_long, align 8
  %16 = alloca %__String__charptr_long_long, align 8
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
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @anon.string.3) #2
  %31 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %1, ptr nonnull %2) #2
  %32 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %31)
  call void @_mp__String__void__ctor__string(ptr nonnull %3, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %4, ptr nonnull @anon.string.3) #2
  %33 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %3, ptr nonnull %4) #2
  %.fca.0.extract76 = extractvalue %__String__charptr_long_long %33, 0
  %.fca.1.extract78 = extractvalue %__String__charptr_long_long %33, 1
  %.fca.1.gep79 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 1
  %.fca.2.extract80 = extractvalue %__String__charptr_long_long %33, 2
  %.fca.2.gep81 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 2
  store ptr %.fca.0.extract76, ptr %s1, align 8
  store i64 %.fca.1.extract78, ptr %.fca.1.gep79, align 8
  store i64 %.fca.2.extract80, ptr %.fca.2.gep81, align 8
  %34 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %33)
  store ptr @anon.string.4, ptr %5, align 8
  %35 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr nonnull %s1, ptr nonnull %5) #2
  %36 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %35)
  call void @_mp__String__void__ctor__string(ptr nonnull %6, ptr nonnull @anon.string.5) #2
  %37 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %6, ptr nonnull %s1) #2
  %38 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %37)
  %39 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %s1, ptr nonnull %s1) #2
  %40 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %39)
  store ptr @anon.string.6, ptr %7, align 8
  %41 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr nonnull %s1, ptr nonnull %7) #2
  %.fca.0.extract70 = extractvalue %__String__charptr_long_long %41, 0
  store ptr %.fca.0.extract70, ptr %8, align 8
  %.fca.1.extract72 = extractvalue %__String__charptr_long_long %41, 1
  %.fca.1.gep73 = getelementptr inbounds %__String__charptr_long_long, ptr %8, i64 0, i32 1
  store i64 %.fca.1.extract72, ptr %.fca.1.gep73, align 8
  %.fca.2.extract74 = extractvalue %__String__charptr_long_long %41, 2
  %.fca.2.gep75 = getelementptr inbounds %__String__charptr_long_long, ptr %8, i64 0, i32 2
  store i64 %.fca.2.extract74, ptr %.fca.2.gep75, align 8
  %42 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %8, ptr nonnull %s1) #2
  %43 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %42)
  call void @_mp__String__void__ctor__string(ptr nonnull %9, ptr nonnull @anon.string.7) #2
  %44 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %9, ptr nonnull %s1) #2
  %.fca.0.extract64 = extractvalue %__String__charptr_long_long %44, 0
  store ptr %.fca.0.extract64, ptr %10, align 8
  %.fca.1.extract66 = extractvalue %__String__charptr_long_long %44, 1
  %.fca.1.gep67 = getelementptr inbounds %__String__charptr_long_long, ptr %10, i64 0, i32 1
  store i64 %.fca.1.extract66, ptr %.fca.1.gep67, align 8
  %.fca.2.extract68 = extractvalue %__String__charptr_long_long %44, 2
  %.fca.2.gep69 = getelementptr inbounds %__String__charptr_long_long, ptr %10, i64 0, i32 2
  store i64 %.fca.2.extract68, ptr %.fca.2.gep69, align 8
  store ptr @anon.string.8, ptr %11, align 8
  %45 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr nonnull %10, ptr nonnull %11) #2
  %46 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %45)
  call void @_mp__String__void__ctor__string(ptr nonnull %12, ptr nonnull @anon.string.38) #2
  %47 = call %__String__charptr_long_long @_f__void__String__op.mul__short_Stringref(i16 4, ptr nonnull %12) #2
  %48 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %47)
  call void @_mp__String__void__ctor__string(ptr nonnull %13, ptr nonnull @anon.string.10) #2
  %49 = call %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr nonnull %13, i32 5) #2
  %50 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %49)
  call void @_mp__String__void__ctor__char(ptr nonnull %14, i8 97) #2
  %51 = call %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32 20, ptr nonnull %14) #2
  %52 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %51)
  call void @_mp__String__void__ctor__char(ptr nonnull %15, i8 99) #2
  %53 = call %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32 2, ptr nonnull %15) #2
  %.fca.0.extract37 = extractvalue %__String__charptr_long_long %53, 0
  store ptr %.fca.0.extract37, ptr %16, align 8
  %.fca.1.extract39 = extractvalue %__String__charptr_long_long %53, 1
  %.fca.1.gep40 = getelementptr inbounds %__String__charptr_long_long, ptr %16, i64 0, i32 1
  store i64 %.fca.1.extract39, ptr %.fca.1.gep40, align 8
  %.fca.2.extract41 = extractvalue %__String__charptr_long_long %53, 2
  %.fca.2.gep42 = getelementptr inbounds %__String__charptr_long_long, ptr %16, i64 0, i32 2
  store i64 %.fca.2.extract41, ptr %.fca.2.gep42, align 8
  %54 = call %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr nonnull %16, i32 7) #2
  %55 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %54)
  %56 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.29, ptr nonnull @anon.string.30) #2
  %57 = zext i1 %56 to i32
  %58 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %57)
  %59 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.35, ptr nonnull @anon.string.32) #2
  %60 = zext i1 %59 to i32
  %61 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %60)
  %62 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.35, ptr nonnull @anon.string.35) #2
  %63 = zext i1 %62 to i32
  %64 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %63)
  call void @_mp__String__void__ctor__string(ptr nonnull %17, ptr nonnull @anon.string.29) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %18, ptr nonnull @anon.string.30) #2
  %65 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr nonnull %17, ptr nonnull %18) #2
  %66 = zext i1 %65 to i32
  %67 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %66)
  call void @_mp__String__void__ctor__string(ptr nonnull %19, ptr nonnull @anon.string.35) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %20, ptr nonnull @anon.string.32) #2
  %68 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr nonnull %19, ptr nonnull %20) #2
  %69 = zext i1 %68 to i32
  %70 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %69)
  call void @_mp__String__void__ctor__string(ptr nonnull %21, ptr nonnull @anon.string.35) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %22, ptr nonnull @anon.string.35) #2
  %71 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr nonnull %21, ptr nonnull %22) #2
  %72 = zext i1 %71 to i32
  %73 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %72)
  %74 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.29, ptr nonnull @anon.string.30) #2
  %75 = xor i1 %74, true
  %76 = zext i1 %75 to i32
  %77 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %76)
  %78 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.35, ptr nonnull @anon.string.32) #2
  %79 = xor i1 %78, true
  %80 = zext i1 %79 to i32
  %81 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %80)
  %82 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.35, ptr nonnull @anon.string.35) #2
  %83 = xor i1 %82, true
  %84 = zext i1 %83 to i32
  %85 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %84)
  call void @_mp__String__void__ctor__string(ptr nonnull %23, ptr nonnull @anon.string.29) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %24, ptr nonnull @anon.string.30) #2
  %86 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr nonnull %23, ptr nonnull %24) #2
  %87 = zext i1 %86 to i32
  %88 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %87)
  call void @_mp__String__void__ctor__string(ptr nonnull %25, ptr nonnull @anon.string.35) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %26, ptr nonnull @anon.string.32) #2
  %89 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr nonnull %25, ptr nonnull %26) #2
  %90 = zext i1 %89 to i32
  %91 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %90)
  call void @_mp__String__void__ctor__string(ptr nonnull %27, ptr nonnull @anon.string.35) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %28, ptr nonnull @anon.string.35) #2
  %92 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr nonnull %27, ptr nonnull %28) #2
  %93 = zext i1 %92 to i32
  %94 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %93)
  call void @_mp__String__void__ctor__string(ptr nonnull %s4, ptr nonnull @anon.string.35) #2
  store i8 108, ptr %29, align 1
  call void @_p__void__void__op.plusequal__Stringref_charref(ptr nonnull %s4, ptr nonnull %29) #2
  %.fca.0.load11 = load ptr, ptr %s4, align 8
  %.fca.0.insert12 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load11, 0
  %.fca.1.gep13 = getelementptr inbounds %__String__charptr_long_long, ptr %s4, i64 0, i32 1
  %.fca.1.load14 = load i64, ptr %.fca.1.gep13, align 8
  %.fca.1.insert15 = insertvalue %__String__charptr_long_long %.fca.0.insert12, i64 %.fca.1.load14, 1
  %.fca.2.gep16 = getelementptr inbounds %__String__charptr_long_long, ptr %s4, i64 0, i32 2
  %.fca.2.load17 = load i64, ptr %.fca.2.gep16, align 8
  %.fca.2.insert18 = insertvalue %__String__charptr_long_long %.fca.1.insert15, i64 %.fca.2.load17, 2
  %95 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %.fca.2.insert18)
  call void @_mp__String__void__ctor__string(ptr nonnull %s5, ptr nonnull @anon.string.38) #2
  store ptr @anon.string.37, ptr %30, align 8
  call void @_p__void__void__op.plusequal__Stringref_stringref(ptr nonnull %s5, ptr nonnull %30) #2
  %.fca.0.load2 = load ptr, ptr %s5, align 8
  %.fca.0.insert3 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load2, 0
  %.fca.1.gep4 = getelementptr inbounds %__String__charptr_long_long, ptr %s5, i64 0, i32 1
  %.fca.1.load5 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert6 = insertvalue %__String__charptr_long_long %.fca.0.insert3, i64 %.fca.1.load5, 1
  %.fca.2.gep7 = getelementptr inbounds %__String__charptr_long_long, ptr %s5, i64 0, i32 2
  %.fca.2.load8 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert9 = insertvalue %__String__charptr_long_long %.fca.1.insert6, i64 %.fca.2.load8, 2
  %96 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %.fca.2.insert9)
  call void @_mp__String__void__ctor__string(ptr nonnull %s6, ptr nonnull @anon.string.38) #2
  call void @_p__void__void__op.mulequal__Stringref_int(ptr nonnull %s6, i32 3) #2
  %.fca.0.load = load ptr, ptr %s6, align 8
  %.fca.0.insert = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s6, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %__String__charptr_long_long %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s6, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %__String__charptr_long_long %.fca.1.insert, i64 %.fca.2.load, 2
  %97 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %.fca.2.insert)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__short_Stringref(i16, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr, i32) local_unnamed_addr

declare void @_mp__String__void__ctor__char(ptr, i8) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32, ptr) local_unnamed_addr

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr) local_unnamed_addr

declare i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr, ptr) local_unnamed_addr

declare i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__op.plusequal__Stringref_charref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__op.plusequal__Stringref_stringref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__op.mulequal__Stringref_int(ptr, i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
