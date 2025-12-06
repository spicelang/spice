; ModuleID = 'source.spice'
source_filename = "source.spice"

@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @tysan.module_ctor, ptr null }]
@__tysan_v1_Simple_20Spice_20TBAA = linkonce_odr constant { i64, i64, [18 x i8] } { i64 2, i64 0, [18 x i8] c"Simple Spice TBAA\00" }
@__tysan_v1_omnipotent_20byte = linkonce_odr constant { i64, i64, ptr, i64, [16 x i8] } { i64 2, i64 1, ptr @__tysan_v1_Simple_20Spice_20TBAA, i64 0, [16 x i8] c"omnipotent byte\00" }
@__tysan_v1_int = linkonce_odr constant { i64, i64, ptr, i64, [4 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [4 x i8] c"int\00" }
@__tysan_v1_int_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_int, ptr @__tysan_v1_int, i64 0 }
@__tysan_v1_long = linkonce_odr constant { i64, i64, ptr, i64, [5 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [5 x i8] c"long\00" }
@__tysan_v1_long_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_long, ptr @__tysan_v1_long, i64 0 }
@__tysan_v1_double_2a = linkonce_odr constant { i64, i64, ptr, i64, [8 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [8 x i8] c"double*\00" }
@__tysan_v1_double_2a_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_double_2a, ptr @__tysan_v1_double_2a, i64 0 }
@__tysan_v1_double = linkonce_odr constant { i64, i64, ptr, i64, [7 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [7 x i8] c"double\00" }
@__tysan_v1_double_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_double, ptr @__tysan_v1_double, i64 0 }
@llvm.used = appending global [11 x ptr] [ptr @tysan.module_ctor, ptr @__tysan_v1_Simple_20Spice_20TBAA, ptr @__tysan_v1_omnipotent_20byte, ptr @__tysan_v1_int, ptr @__tysan_v1_int_o_0, ptr @__tysan_v1_long, ptr @__tysan_v1_long_o_0, ptr @__tysan_v1_double_2a, ptr @__tysan_v1_double_2a_o_0, ptr @__tysan_v1_double, ptr @__tysan_v1_double_o_0], section "llvm.metadata"
@__tysan_shadow_memory_address = external global i64
@__tysan_app_memory_mask = external global i64

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_type uwtable
define dso_local noundef i32 @main() #0 !dbg !9 {
  %app.mem.mask = load i64, ptr @__tysan_app_memory_mask, align 8
  %shadow.base = load i64, ptr @__tysan_shadow_memory_address, align 8
  %result = alloca i32, align 4, !type !15
  %1 = ptrtoint ptr %result to i64
  %2 = and i64 %1, %app.mem.mask
  %3 = shl i64 %2, 3
  %4 = add i64 %3, %shadow.base
  %5 = inttoptr i64 %4 to ptr
  call void @llvm.memset.p0.i64(ptr align 8 %5, i8 0, i64 32, i1 false)
  %l = alloca i64, align 8, !type !16
  %6 = ptrtoint ptr %l to i64
  %7 = and i64 %6, %app.mem.mask
  %8 = shl i64 %7, 3
  %9 = add i64 %8, %shadow.base
  %10 = inttoptr i64 %9 to ptr
  call void @llvm.memset.p0.i64(ptr align 8 %10, i8 0, i64 64, i1 false)
  %ptr = alloca ptr, align 8, !dbg !17, !type !18
    #dbg_declare(ptr %result, !19, !DIExpression(), !17)
  %11 = ptrtoint ptr %ptr to i64, !dbg !17
  %12 = and i64 %11, %app.mem.mask, !dbg !17
  %13 = shl i64 %12, 3, !dbg !17
  %14 = add i64 %13, %shadow.base, !dbg !17
  %15 = inttoptr i64 %14 to ptr, !dbg !17
  call void @llvm.memset.p0.i64(ptr align 8 %15, i8 0, i64 64, i1 false), !dbg !17
  %app.ptr.int = ptrtoint ptr %result to i64, !dbg !17
  %app.ptr.masked = and i64 %app.ptr.int, %app.mem.mask, !dbg !17
  %app.ptr.shifted = shl i64 %app.ptr.masked, 3, !dbg !17
  %shadow.ptr.int = add i64 %app.ptr.shifted, %shadow.base, !dbg !17
  %shadow.ptr = inttoptr i64 %shadow.ptr.int to ptr, !dbg !17
  %shadow.desc = load ptr, ptr %shadow.ptr, align 8, !dbg !17
  %bad.desc = icmp ne ptr %shadow.desc, @__tysan_v1_int_o_0, !dbg !17
  br i1 %bad.desc, label %16, label %38, !dbg !17, !prof !20

16:                                               ; preds = %0
  %17 = icmp eq ptr %shadow.desc, null, !dbg !17
  br i1 %17, label %18, label %36, !dbg !17

18:                                               ; preds = %16
  %19 = add i64 %shadow.ptr.int, 8, !dbg !17
  %20 = inttoptr i64 %19 to ptr, !dbg !17
  %21 = load ptr, ptr %20, align 8, !dbg !17
  %22 = icmp ne ptr %21, null, !dbg !17
  %23 = or i1 false, %22, !dbg !17
  %24 = add i64 %shadow.ptr.int, 16, !dbg !17
  %25 = inttoptr i64 %24 to ptr, !dbg !17
  %26 = load ptr, ptr %25, align 8, !dbg !17
  %27 = icmp ne ptr %26, null, !dbg !17
  %28 = or i1 %23, %27, !dbg !17
  %29 = add i64 %shadow.ptr.int, 24, !dbg !17
  %30 = inttoptr i64 %29 to ptr, !dbg !17
  %31 = load ptr, ptr %30, align 8, !dbg !17
  %32 = icmp ne ptr %31, null, !dbg !17
  %33 = or i1 %28, %32, !dbg !17
  br i1 %33, label %34, label %35, !dbg !17, !prof !20

34:                                               ; preds = %18
  call void @__tysan_check(ptr %result, i32 4, ptr @__tysan_v1_int_o_0, i32 2), !dbg !17
  br label %35, !dbg !17

35:                                               ; preds = %18, %34
  store ptr @__tysan_v1_int_o_0, ptr %shadow.ptr, align 8, !dbg !17
  %shadow.byte.1.offset = add i64 %shadow.ptr.int, 8, !dbg !17
  %shadow.byte.1.ptr = inttoptr i64 %shadow.byte.1.offset to ptr, !dbg !17
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr, align 8, !dbg !17
  %shadow.byte.2.offset = add i64 %shadow.ptr.int, 16, !dbg !17
  %shadow.byte.2.ptr = inttoptr i64 %shadow.byte.2.offset to ptr, !dbg !17
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr, align 8, !dbg !17
  %shadow.byte.3.offset = add i64 %shadow.ptr.int, 24, !dbg !17
  %shadow.byte.3.ptr = inttoptr i64 %shadow.byte.3.offset to ptr, !dbg !17
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr, align 8, !dbg !17
  br label %37, !dbg !17

36:                                               ; preds = %16
  call void @__tysan_check(ptr %result, i32 4, ptr @__tysan_v1_int_o_0, i32 2), !dbg !17
  br label %37, !dbg !17

37:                                               ; preds = %36, %35
  br label %59, !dbg !17

38:                                               ; preds = %0
  %39 = add i64 %shadow.ptr.int, 8, !dbg !17
  %40 = inttoptr i64 %39 to ptr, !dbg !17
  %41 = load ptr, ptr %40, align 8, !dbg !17
  %42 = ptrtoint ptr %41 to i64, !dbg !17
  %43 = icmp sge i64 %42, 0, !dbg !17
  %44 = or i1 false, %43, !dbg !17
  %45 = add i64 %shadow.ptr.int, 16, !dbg !17
  %46 = inttoptr i64 %45 to ptr, !dbg !17
  %47 = load ptr, ptr %46, align 8, !dbg !17
  %48 = ptrtoint ptr %47 to i64, !dbg !17
  %49 = icmp sge i64 %48, 0, !dbg !17
  %50 = or i1 %44, %49, !dbg !17
  %51 = add i64 %shadow.ptr.int, 24, !dbg !17
  %52 = inttoptr i64 %51 to ptr, !dbg !17
  %53 = load ptr, ptr %52, align 8, !dbg !17
  %54 = ptrtoint ptr %53 to i64, !dbg !17
  %55 = icmp sge i64 %54, 0, !dbg !17
  %56 = or i1 %50, %55, !dbg !17
  br i1 %56, label %57, label %58, !dbg !17, !prof !20

57:                                               ; preds = %38
  call void @__tysan_check(ptr %result, i32 4, ptr @__tysan_v1_int_o_0, i32 2), !dbg !17
  br label %58, !dbg !17

58:                                               ; preds = %38, %57
  br label %59, !dbg !17

59:                                               ; preds = %58, %37
  store i32 0, ptr %result, align 4, !dbg !17, !tbaa !21
  %app.ptr.int1 = ptrtoint ptr %l to i64, !dbg !25
  %app.ptr.masked2 = and i64 %app.ptr.int1, %app.mem.mask, !dbg !25
  %app.ptr.shifted3 = shl i64 %app.ptr.masked2, 3, !dbg !25
  %shadow.ptr.int4 = add i64 %app.ptr.shifted3, %shadow.base, !dbg !25
  %shadow.ptr5 = inttoptr i64 %shadow.ptr.int4 to ptr, !dbg !25
  %shadow.desc6 = load ptr, ptr %shadow.ptr5, align 8, !dbg !25
  %bad.desc7 = icmp ne ptr %shadow.desc6, @__tysan_v1_long_o_0, !dbg !25
  br i1 %bad.desc7, label %60, label %102, !dbg !25, !prof !20

60:                                               ; preds = %59
  %61 = icmp eq ptr %shadow.desc6, null, !dbg !25
  br i1 %61, label %62, label %100, !dbg !25

62:                                               ; preds = %60
  %63 = add i64 %shadow.ptr.int4, 8, !dbg !25
  %64 = inttoptr i64 %63 to ptr, !dbg !25
  %65 = load ptr, ptr %64, align 8, !dbg !25
  %66 = icmp ne ptr %65, null, !dbg !25
  %67 = or i1 false, %66, !dbg !25
  %68 = add i64 %shadow.ptr.int4, 16, !dbg !25
  %69 = inttoptr i64 %68 to ptr, !dbg !25
  %70 = load ptr, ptr %69, align 8, !dbg !25
  %71 = icmp ne ptr %70, null, !dbg !25
  %72 = or i1 %67, %71, !dbg !25
  %73 = add i64 %shadow.ptr.int4, 24, !dbg !25
  %74 = inttoptr i64 %73 to ptr, !dbg !25
  %75 = load ptr, ptr %74, align 8, !dbg !25
  %76 = icmp ne ptr %75, null, !dbg !25
  %77 = or i1 %72, %76, !dbg !25
  %78 = add i64 %shadow.ptr.int4, 32, !dbg !25
  %79 = inttoptr i64 %78 to ptr, !dbg !25
  %80 = load ptr, ptr %79, align 8, !dbg !25
  %81 = icmp ne ptr %80, null, !dbg !25
  %82 = or i1 %77, %81, !dbg !25
  %83 = add i64 %shadow.ptr.int4, 40, !dbg !25
  %84 = inttoptr i64 %83 to ptr, !dbg !25
  %85 = load ptr, ptr %84, align 8, !dbg !25
  %86 = icmp ne ptr %85, null, !dbg !25
  %87 = or i1 %82, %86, !dbg !25
  %88 = add i64 %shadow.ptr.int4, 48, !dbg !25
  %89 = inttoptr i64 %88 to ptr, !dbg !25
  %90 = load ptr, ptr %89, align 8, !dbg !25
  %91 = icmp ne ptr %90, null, !dbg !25
  %92 = or i1 %87, %91, !dbg !25
  %93 = add i64 %shadow.ptr.int4, 56, !dbg !25
  %94 = inttoptr i64 %93 to ptr, !dbg !25
  %95 = load ptr, ptr %94, align 8, !dbg !25
  %96 = icmp ne ptr %95, null, !dbg !25
  %97 = or i1 %92, %96, !dbg !25
  br i1 %97, label %98, label %99, !dbg !25, !prof !20

98:                                               ; preds = %62
  call void @__tysan_check(ptr %l, i32 8, ptr @__tysan_v1_long_o_0, i32 2), !dbg !25
  br label %99, !dbg !25

99:                                               ; preds = %62, %98
  store ptr @__tysan_v1_long_o_0, ptr %shadow.ptr5, align 8, !dbg !25
  %shadow.byte.1.offset8 = add i64 %shadow.ptr.int4, 8, !dbg !25
  %shadow.byte.1.ptr9 = inttoptr i64 %shadow.byte.1.offset8 to ptr, !dbg !25
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr9, align 8, !dbg !25
  %shadow.byte.2.offset10 = add i64 %shadow.ptr.int4, 16, !dbg !25
  %shadow.byte.2.ptr11 = inttoptr i64 %shadow.byte.2.offset10 to ptr, !dbg !25
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr11, align 8, !dbg !25
  %shadow.byte.3.offset12 = add i64 %shadow.ptr.int4, 24, !dbg !25
  %shadow.byte.3.ptr13 = inttoptr i64 %shadow.byte.3.offset12 to ptr, !dbg !25
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr13, align 8, !dbg !25
  %shadow.byte.4.offset = add i64 %shadow.ptr.int4, 32, !dbg !25
  %shadow.byte.4.ptr = inttoptr i64 %shadow.byte.4.offset to ptr, !dbg !25
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr, align 8, !dbg !25
  %shadow.byte.5.offset = add i64 %shadow.ptr.int4, 40, !dbg !25
  %shadow.byte.5.ptr = inttoptr i64 %shadow.byte.5.offset to ptr, !dbg !25
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr, align 8, !dbg !25
  %shadow.byte.6.offset = add i64 %shadow.ptr.int4, 48, !dbg !25
  %shadow.byte.6.ptr = inttoptr i64 %shadow.byte.6.offset to ptr, !dbg !25
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr, align 8, !dbg !25
  %shadow.byte.7.offset = add i64 %shadow.ptr.int4, 56, !dbg !25
  %shadow.byte.7.ptr = inttoptr i64 %shadow.byte.7.offset to ptr, !dbg !25
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr, align 8, !dbg !25
  br label %101, !dbg !25

100:                                              ; preds = %60
  call void @__tysan_check(ptr %l, i32 8, ptr @__tysan_v1_long_o_0, i32 2), !dbg !25
  br label %101, !dbg !25

101:                                              ; preds = %100, %99
  br label %147, !dbg !25

102:                                              ; preds = %59
  %103 = add i64 %shadow.ptr.int4, 8, !dbg !25
  %104 = inttoptr i64 %103 to ptr, !dbg !25
  %105 = load ptr, ptr %104, align 8, !dbg !25
  %106 = ptrtoint ptr %105 to i64, !dbg !25
  %107 = icmp sge i64 %106, 0, !dbg !25
  %108 = or i1 false, %107, !dbg !25
  %109 = add i64 %shadow.ptr.int4, 16, !dbg !25
  %110 = inttoptr i64 %109 to ptr, !dbg !25
  %111 = load ptr, ptr %110, align 8, !dbg !25
  %112 = ptrtoint ptr %111 to i64, !dbg !25
  %113 = icmp sge i64 %112, 0, !dbg !25
  %114 = or i1 %108, %113, !dbg !25
  %115 = add i64 %shadow.ptr.int4, 24, !dbg !25
  %116 = inttoptr i64 %115 to ptr, !dbg !25
  %117 = load ptr, ptr %116, align 8, !dbg !25
  %118 = ptrtoint ptr %117 to i64, !dbg !25
  %119 = icmp sge i64 %118, 0, !dbg !25
  %120 = or i1 %114, %119, !dbg !25
  %121 = add i64 %shadow.ptr.int4, 32, !dbg !25
  %122 = inttoptr i64 %121 to ptr, !dbg !25
  %123 = load ptr, ptr %122, align 8, !dbg !25
  %124 = ptrtoint ptr %123 to i64, !dbg !25
  %125 = icmp sge i64 %124, 0, !dbg !25
  %126 = or i1 %120, %125, !dbg !25
  %127 = add i64 %shadow.ptr.int4, 40, !dbg !25
  %128 = inttoptr i64 %127 to ptr, !dbg !25
  %129 = load ptr, ptr %128, align 8, !dbg !25
  %130 = ptrtoint ptr %129 to i64, !dbg !25
  %131 = icmp sge i64 %130, 0, !dbg !25
  %132 = or i1 %126, %131, !dbg !25
  %133 = add i64 %shadow.ptr.int4, 48, !dbg !25
  %134 = inttoptr i64 %133 to ptr, !dbg !25
  %135 = load ptr, ptr %134, align 8, !dbg !25
  %136 = ptrtoint ptr %135 to i64, !dbg !25
  %137 = icmp sge i64 %136, 0, !dbg !25
  %138 = or i1 %132, %137, !dbg !25
  %139 = add i64 %shadow.ptr.int4, 56, !dbg !25
  %140 = inttoptr i64 %139 to ptr, !dbg !25
  %141 = load ptr, ptr %140, align 8, !dbg !25
  %142 = ptrtoint ptr %141 to i64, !dbg !25
  %143 = icmp sge i64 %142, 0, !dbg !25
  %144 = or i1 %138, %143, !dbg !25
  br i1 %144, label %145, label %146, !dbg !25, !prof !20

145:                                              ; preds = %102
  call void @__tysan_check(ptr %l, i32 8, ptr @__tysan_v1_long_o_0, i32 2), !dbg !25
  br label %146, !dbg !25

146:                                              ; preds = %102, %145
  br label %147, !dbg !25

147:                                              ; preds = %146, %101
  store i64 100, ptr %l, align 8, !dbg !25, !tbaa !26
    #dbg_declare(ptr %l, !28, !DIExpression(), !30)
  %app.ptr.int14 = ptrtoint ptr %ptr to i64, !dbg !31
  %app.ptr.masked15 = and i64 %app.ptr.int14, %app.mem.mask, !dbg !31
  %app.ptr.shifted16 = shl i64 %app.ptr.masked15, 3, !dbg !31
  %shadow.ptr.int17 = add i64 %app.ptr.shifted16, %shadow.base, !dbg !31
  %shadow.ptr18 = inttoptr i64 %shadow.ptr.int17 to ptr, !dbg !31
  %shadow.desc19 = load ptr, ptr %shadow.ptr18, align 8, !dbg !31
  %bad.desc20 = icmp ne ptr %shadow.desc19, @__tysan_v1_double_2a_o_0, !dbg !31
  br i1 %bad.desc20, label %148, label %190, !dbg !31, !prof !20

148:                                              ; preds = %147
  %149 = icmp eq ptr %shadow.desc19, null, !dbg !31
  br i1 %149, label %150, label %188, !dbg !31

150:                                              ; preds = %148
  %151 = add i64 %shadow.ptr.int17, 8, !dbg !31
  %152 = inttoptr i64 %151 to ptr, !dbg !31
  %153 = load ptr, ptr %152, align 8, !dbg !31
  %154 = icmp ne ptr %153, null, !dbg !31
  %155 = or i1 false, %154, !dbg !31
  %156 = add i64 %shadow.ptr.int17, 16, !dbg !31
  %157 = inttoptr i64 %156 to ptr, !dbg !31
  %158 = load ptr, ptr %157, align 8, !dbg !31
  %159 = icmp ne ptr %158, null, !dbg !31
  %160 = or i1 %155, %159, !dbg !31
  %161 = add i64 %shadow.ptr.int17, 24, !dbg !31
  %162 = inttoptr i64 %161 to ptr, !dbg !31
  %163 = load ptr, ptr %162, align 8, !dbg !31
  %164 = icmp ne ptr %163, null, !dbg !31
  %165 = or i1 %160, %164, !dbg !31
  %166 = add i64 %shadow.ptr.int17, 32, !dbg !31
  %167 = inttoptr i64 %166 to ptr, !dbg !31
  %168 = load ptr, ptr %167, align 8, !dbg !31
  %169 = icmp ne ptr %168, null, !dbg !31
  %170 = or i1 %165, %169, !dbg !31
  %171 = add i64 %shadow.ptr.int17, 40, !dbg !31
  %172 = inttoptr i64 %171 to ptr, !dbg !31
  %173 = load ptr, ptr %172, align 8, !dbg !31
  %174 = icmp ne ptr %173, null, !dbg !31
  %175 = or i1 %170, %174, !dbg !31
  %176 = add i64 %shadow.ptr.int17, 48, !dbg !31
  %177 = inttoptr i64 %176 to ptr, !dbg !31
  %178 = load ptr, ptr %177, align 8, !dbg !31
  %179 = icmp ne ptr %178, null, !dbg !31
  %180 = or i1 %175, %179, !dbg !31
  %181 = add i64 %shadow.ptr.int17, 56, !dbg !31
  %182 = inttoptr i64 %181 to ptr, !dbg !31
  %183 = load ptr, ptr %182, align 8, !dbg !31
  %184 = icmp ne ptr %183, null, !dbg !31
  %185 = or i1 %180, %184, !dbg !31
  br i1 %185, label %186, label %187, !dbg !31, !prof !20

186:                                              ; preds = %150
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 2), !dbg !31
  br label %187, !dbg !31

187:                                              ; preds = %150, %186
  store ptr @__tysan_v1_double_2a_o_0, ptr %shadow.ptr18, align 8, !dbg !31
  %shadow.byte.1.offset21 = add i64 %shadow.ptr.int17, 8, !dbg !31
  %shadow.byte.1.ptr22 = inttoptr i64 %shadow.byte.1.offset21 to ptr, !dbg !31
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr22, align 8, !dbg !31
  %shadow.byte.2.offset23 = add i64 %shadow.ptr.int17, 16, !dbg !31
  %shadow.byte.2.ptr24 = inttoptr i64 %shadow.byte.2.offset23 to ptr, !dbg !31
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr24, align 8, !dbg !31
  %shadow.byte.3.offset25 = add i64 %shadow.ptr.int17, 24, !dbg !31
  %shadow.byte.3.ptr26 = inttoptr i64 %shadow.byte.3.offset25 to ptr, !dbg !31
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr26, align 8, !dbg !31
  %shadow.byte.4.offset27 = add i64 %shadow.ptr.int17, 32, !dbg !31
  %shadow.byte.4.ptr28 = inttoptr i64 %shadow.byte.4.offset27 to ptr, !dbg !31
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr28, align 8, !dbg !31
  %shadow.byte.5.offset29 = add i64 %shadow.ptr.int17, 40, !dbg !31
  %shadow.byte.5.ptr30 = inttoptr i64 %shadow.byte.5.offset29 to ptr, !dbg !31
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr30, align 8, !dbg !31
  %shadow.byte.6.offset31 = add i64 %shadow.ptr.int17, 48, !dbg !31
  %shadow.byte.6.ptr32 = inttoptr i64 %shadow.byte.6.offset31 to ptr, !dbg !31
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr32, align 8, !dbg !31
  %shadow.byte.7.offset33 = add i64 %shadow.ptr.int17, 56, !dbg !31
  %shadow.byte.7.ptr34 = inttoptr i64 %shadow.byte.7.offset33 to ptr, !dbg !31
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr34, align 8, !dbg !31
  br label %189, !dbg !31

188:                                              ; preds = %148
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 2), !dbg !31
  br label %189, !dbg !31

189:                                              ; preds = %188, %187
  br label %235, !dbg !31

190:                                              ; preds = %147
  %191 = add i64 %shadow.ptr.int17, 8, !dbg !31
  %192 = inttoptr i64 %191 to ptr, !dbg !31
  %193 = load ptr, ptr %192, align 8, !dbg !31
  %194 = ptrtoint ptr %193 to i64, !dbg !31
  %195 = icmp sge i64 %194, 0, !dbg !31
  %196 = or i1 false, %195, !dbg !31
  %197 = add i64 %shadow.ptr.int17, 16, !dbg !31
  %198 = inttoptr i64 %197 to ptr, !dbg !31
  %199 = load ptr, ptr %198, align 8, !dbg !31
  %200 = ptrtoint ptr %199 to i64, !dbg !31
  %201 = icmp sge i64 %200, 0, !dbg !31
  %202 = or i1 %196, %201, !dbg !31
  %203 = add i64 %shadow.ptr.int17, 24, !dbg !31
  %204 = inttoptr i64 %203 to ptr, !dbg !31
  %205 = load ptr, ptr %204, align 8, !dbg !31
  %206 = ptrtoint ptr %205 to i64, !dbg !31
  %207 = icmp sge i64 %206, 0, !dbg !31
  %208 = or i1 %202, %207, !dbg !31
  %209 = add i64 %shadow.ptr.int17, 32, !dbg !31
  %210 = inttoptr i64 %209 to ptr, !dbg !31
  %211 = load ptr, ptr %210, align 8, !dbg !31
  %212 = ptrtoint ptr %211 to i64, !dbg !31
  %213 = icmp sge i64 %212, 0, !dbg !31
  %214 = or i1 %208, %213, !dbg !31
  %215 = add i64 %shadow.ptr.int17, 40, !dbg !31
  %216 = inttoptr i64 %215 to ptr, !dbg !31
  %217 = load ptr, ptr %216, align 8, !dbg !31
  %218 = ptrtoint ptr %217 to i64, !dbg !31
  %219 = icmp sge i64 %218, 0, !dbg !31
  %220 = or i1 %214, %219, !dbg !31
  %221 = add i64 %shadow.ptr.int17, 48, !dbg !31
  %222 = inttoptr i64 %221 to ptr, !dbg !31
  %223 = load ptr, ptr %222, align 8, !dbg !31
  %224 = ptrtoint ptr %223 to i64, !dbg !31
  %225 = icmp sge i64 %224, 0, !dbg !31
  %226 = or i1 %220, %225, !dbg !31
  %227 = add i64 %shadow.ptr.int17, 56, !dbg !31
  %228 = inttoptr i64 %227 to ptr, !dbg !31
  %229 = load ptr, ptr %228, align 8, !dbg !31
  %230 = ptrtoint ptr %229 to i64, !dbg !31
  %231 = icmp sge i64 %230, 0, !dbg !31
  %232 = or i1 %226, %231, !dbg !31
  br i1 %232, label %233, label %234, !dbg !31, !prof !20

233:                                              ; preds = %190
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 2), !dbg !31
  br label %234, !dbg !31

234:                                              ; preds = %190, %233
  br label %235, !dbg !31

235:                                              ; preds = %234, %189
  store ptr %l, ptr %ptr, align 8, !dbg !31, !tbaa !33
    #dbg_declare(ptr %ptr, !35, !DIExpression(), !38)
  %app.ptr.int35 = ptrtoint ptr %ptr to i64, !dbg !39
  %app.ptr.masked36 = and i64 %app.ptr.int35, %app.mem.mask, !dbg !39
  %app.ptr.shifted37 = shl i64 %app.ptr.masked36, 3, !dbg !39
  %shadow.ptr.int38 = add i64 %app.ptr.shifted37, %shadow.base, !dbg !39
  %shadow.ptr39 = inttoptr i64 %shadow.ptr.int38 to ptr, !dbg !39
  %shadow.desc40 = load ptr, ptr %shadow.ptr39, align 8, !dbg !39
  %bad.desc41 = icmp ne ptr %shadow.desc40, @__tysan_v1_double_2a_o_0, !dbg !39
  br i1 %bad.desc41, label %236, label %278, !dbg !39, !prof !20

236:                                              ; preds = %235
  %237 = icmp eq ptr %shadow.desc40, null, !dbg !39
  br i1 %237, label %238, label %276, !dbg !39

238:                                              ; preds = %236
  %239 = add i64 %shadow.ptr.int38, 8, !dbg !39
  %240 = inttoptr i64 %239 to ptr, !dbg !39
  %241 = load ptr, ptr %240, align 8, !dbg !39
  %242 = icmp ne ptr %241, null, !dbg !39
  %243 = or i1 false, %242, !dbg !39
  %244 = add i64 %shadow.ptr.int38, 16, !dbg !39
  %245 = inttoptr i64 %244 to ptr, !dbg !39
  %246 = load ptr, ptr %245, align 8, !dbg !39
  %247 = icmp ne ptr %246, null, !dbg !39
  %248 = or i1 %243, %247, !dbg !39
  %249 = add i64 %shadow.ptr.int38, 24, !dbg !39
  %250 = inttoptr i64 %249 to ptr, !dbg !39
  %251 = load ptr, ptr %250, align 8, !dbg !39
  %252 = icmp ne ptr %251, null, !dbg !39
  %253 = or i1 %248, %252, !dbg !39
  %254 = add i64 %shadow.ptr.int38, 32, !dbg !39
  %255 = inttoptr i64 %254 to ptr, !dbg !39
  %256 = load ptr, ptr %255, align 8, !dbg !39
  %257 = icmp ne ptr %256, null, !dbg !39
  %258 = or i1 %253, %257, !dbg !39
  %259 = add i64 %shadow.ptr.int38, 40, !dbg !39
  %260 = inttoptr i64 %259 to ptr, !dbg !39
  %261 = load ptr, ptr %260, align 8, !dbg !39
  %262 = icmp ne ptr %261, null, !dbg !39
  %263 = or i1 %258, %262, !dbg !39
  %264 = add i64 %shadow.ptr.int38, 48, !dbg !39
  %265 = inttoptr i64 %264 to ptr, !dbg !39
  %266 = load ptr, ptr %265, align 8, !dbg !39
  %267 = icmp ne ptr %266, null, !dbg !39
  %268 = or i1 %263, %267, !dbg !39
  %269 = add i64 %shadow.ptr.int38, 56, !dbg !39
  %270 = inttoptr i64 %269 to ptr, !dbg !39
  %271 = load ptr, ptr %270, align 8, !dbg !39
  %272 = icmp ne ptr %271, null, !dbg !39
  %273 = or i1 %268, %272, !dbg !39
  br i1 %273, label %274, label %275, !dbg !39, !prof !20

274:                                              ; preds = %238
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 1), !dbg !39
  br label %275, !dbg !39

275:                                              ; preds = %238, %274
  store ptr @__tysan_v1_double_2a_o_0, ptr %shadow.ptr39, align 8, !dbg !39
  %shadow.byte.1.offset42 = add i64 %shadow.ptr.int38, 8, !dbg !39
  %shadow.byte.1.ptr43 = inttoptr i64 %shadow.byte.1.offset42 to ptr, !dbg !39
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr43, align 8, !dbg !39
  %shadow.byte.2.offset44 = add i64 %shadow.ptr.int38, 16, !dbg !39
  %shadow.byte.2.ptr45 = inttoptr i64 %shadow.byte.2.offset44 to ptr, !dbg !39
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr45, align 8, !dbg !39
  %shadow.byte.3.offset46 = add i64 %shadow.ptr.int38, 24, !dbg !39
  %shadow.byte.3.ptr47 = inttoptr i64 %shadow.byte.3.offset46 to ptr, !dbg !39
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr47, align 8, !dbg !39
  %shadow.byte.4.offset48 = add i64 %shadow.ptr.int38, 32, !dbg !39
  %shadow.byte.4.ptr49 = inttoptr i64 %shadow.byte.4.offset48 to ptr, !dbg !39
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr49, align 8, !dbg !39
  %shadow.byte.5.offset50 = add i64 %shadow.ptr.int38, 40, !dbg !39
  %shadow.byte.5.ptr51 = inttoptr i64 %shadow.byte.5.offset50 to ptr, !dbg !39
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr51, align 8, !dbg !39
  %shadow.byte.6.offset52 = add i64 %shadow.ptr.int38, 48, !dbg !39
  %shadow.byte.6.ptr53 = inttoptr i64 %shadow.byte.6.offset52 to ptr, !dbg !39
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr53, align 8, !dbg !39
  %shadow.byte.7.offset54 = add i64 %shadow.ptr.int38, 56, !dbg !39
  %shadow.byte.7.ptr55 = inttoptr i64 %shadow.byte.7.offset54 to ptr, !dbg !39
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr55, align 8, !dbg !39
  br label %277, !dbg !39

276:                                              ; preds = %236
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 1), !dbg !39
  br label %277, !dbg !39

277:                                              ; preds = %276, %275
  br label %323, !dbg !39

278:                                              ; preds = %235
  %279 = add i64 %shadow.ptr.int38, 8, !dbg !39
  %280 = inttoptr i64 %279 to ptr, !dbg !39
  %281 = load ptr, ptr %280, align 8, !dbg !39
  %282 = ptrtoint ptr %281 to i64, !dbg !39
  %283 = icmp sge i64 %282, 0, !dbg !39
  %284 = or i1 false, %283, !dbg !39
  %285 = add i64 %shadow.ptr.int38, 16, !dbg !39
  %286 = inttoptr i64 %285 to ptr, !dbg !39
  %287 = load ptr, ptr %286, align 8, !dbg !39
  %288 = ptrtoint ptr %287 to i64, !dbg !39
  %289 = icmp sge i64 %288, 0, !dbg !39
  %290 = or i1 %284, %289, !dbg !39
  %291 = add i64 %shadow.ptr.int38, 24, !dbg !39
  %292 = inttoptr i64 %291 to ptr, !dbg !39
  %293 = load ptr, ptr %292, align 8, !dbg !39
  %294 = ptrtoint ptr %293 to i64, !dbg !39
  %295 = icmp sge i64 %294, 0, !dbg !39
  %296 = or i1 %290, %295, !dbg !39
  %297 = add i64 %shadow.ptr.int38, 32, !dbg !39
  %298 = inttoptr i64 %297 to ptr, !dbg !39
  %299 = load ptr, ptr %298, align 8, !dbg !39
  %300 = ptrtoint ptr %299 to i64, !dbg !39
  %301 = icmp sge i64 %300, 0, !dbg !39
  %302 = or i1 %296, %301, !dbg !39
  %303 = add i64 %shadow.ptr.int38, 40, !dbg !39
  %304 = inttoptr i64 %303 to ptr, !dbg !39
  %305 = load ptr, ptr %304, align 8, !dbg !39
  %306 = ptrtoint ptr %305 to i64, !dbg !39
  %307 = icmp sge i64 %306, 0, !dbg !39
  %308 = or i1 %302, %307, !dbg !39
  %309 = add i64 %shadow.ptr.int38, 48, !dbg !39
  %310 = inttoptr i64 %309 to ptr, !dbg !39
  %311 = load ptr, ptr %310, align 8, !dbg !39
  %312 = ptrtoint ptr %311 to i64, !dbg !39
  %313 = icmp sge i64 %312, 0, !dbg !39
  %314 = or i1 %308, %313, !dbg !39
  %315 = add i64 %shadow.ptr.int38, 56, !dbg !39
  %316 = inttoptr i64 %315 to ptr, !dbg !39
  %317 = load ptr, ptr %316, align 8, !dbg !39
  %318 = ptrtoint ptr %317 to i64, !dbg !39
  %319 = icmp sge i64 %318, 0, !dbg !39
  %320 = or i1 %314, %319, !dbg !39
  br i1 %320, label %321, label %322, !dbg !39, !prof !20

321:                                              ; preds = %278
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 1), !dbg !39
  br label %322, !dbg !39

322:                                              ; preds = %278, %321
  br label %323, !dbg !39

323:                                              ; preds = %322, %277
  %324 = load ptr, ptr %ptr, align 8, !dbg !39, !tbaa !33
  %app.ptr.int56 = ptrtoint ptr %324 to i64, !dbg !39
  %app.ptr.masked57 = and i64 %app.ptr.int56, %app.mem.mask, !dbg !39
  %app.ptr.shifted58 = shl i64 %app.ptr.masked57, 3, !dbg !39
  %shadow.ptr.int59 = add i64 %app.ptr.shifted58, %shadow.base, !dbg !39
  %shadow.ptr60 = inttoptr i64 %shadow.ptr.int59 to ptr, !dbg !39
  %shadow.desc61 = load ptr, ptr %shadow.ptr60, align 8, !dbg !39
  %bad.desc62 = icmp ne ptr %shadow.desc61, @__tysan_v1_double_o_0, !dbg !39
  br i1 %bad.desc62, label %325, label %367, !dbg !39, !prof !20

325:                                              ; preds = %323
  %326 = icmp eq ptr %shadow.desc61, null, !dbg !39
  br i1 %326, label %327, label %365, !dbg !39

327:                                              ; preds = %325
  %328 = add i64 %shadow.ptr.int59, 8, !dbg !39
  %329 = inttoptr i64 %328 to ptr, !dbg !39
  %330 = load ptr, ptr %329, align 8, !dbg !39
  %331 = icmp ne ptr %330, null, !dbg !39
  %332 = or i1 false, %331, !dbg !39
  %333 = add i64 %shadow.ptr.int59, 16, !dbg !39
  %334 = inttoptr i64 %333 to ptr, !dbg !39
  %335 = load ptr, ptr %334, align 8, !dbg !39
  %336 = icmp ne ptr %335, null, !dbg !39
  %337 = or i1 %332, %336, !dbg !39
  %338 = add i64 %shadow.ptr.int59, 24, !dbg !39
  %339 = inttoptr i64 %338 to ptr, !dbg !39
  %340 = load ptr, ptr %339, align 8, !dbg !39
  %341 = icmp ne ptr %340, null, !dbg !39
  %342 = or i1 %337, %341, !dbg !39
  %343 = add i64 %shadow.ptr.int59, 32, !dbg !39
  %344 = inttoptr i64 %343 to ptr, !dbg !39
  %345 = load ptr, ptr %344, align 8, !dbg !39
  %346 = icmp ne ptr %345, null, !dbg !39
  %347 = or i1 %342, %346, !dbg !39
  %348 = add i64 %shadow.ptr.int59, 40, !dbg !39
  %349 = inttoptr i64 %348 to ptr, !dbg !39
  %350 = load ptr, ptr %349, align 8, !dbg !39
  %351 = icmp ne ptr %350, null, !dbg !39
  %352 = or i1 %347, %351, !dbg !39
  %353 = add i64 %shadow.ptr.int59, 48, !dbg !39
  %354 = inttoptr i64 %353 to ptr, !dbg !39
  %355 = load ptr, ptr %354, align 8, !dbg !39
  %356 = icmp ne ptr %355, null, !dbg !39
  %357 = or i1 %352, %356, !dbg !39
  %358 = add i64 %shadow.ptr.int59, 56, !dbg !39
  %359 = inttoptr i64 %358 to ptr, !dbg !39
  %360 = load ptr, ptr %359, align 8, !dbg !39
  %361 = icmp ne ptr %360, null, !dbg !39
  %362 = or i1 %357, %361, !dbg !39
  br i1 %362, label %363, label %364, !dbg !39, !prof !20

363:                                              ; preds = %327
  call void @__tysan_check(ptr %324, i32 8, ptr @__tysan_v1_double_o_0, i32 1), !dbg !39
  br label %364, !dbg !39

364:                                              ; preds = %327, %363
  store ptr @__tysan_v1_double_o_0, ptr %shadow.ptr60, align 8, !dbg !39
  %shadow.byte.1.offset63 = add i64 %shadow.ptr.int59, 8, !dbg !39
  %shadow.byte.1.ptr64 = inttoptr i64 %shadow.byte.1.offset63 to ptr, !dbg !39
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr64, align 8, !dbg !39
  %shadow.byte.2.offset65 = add i64 %shadow.ptr.int59, 16, !dbg !39
  %shadow.byte.2.ptr66 = inttoptr i64 %shadow.byte.2.offset65 to ptr, !dbg !39
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr66, align 8, !dbg !39
  %shadow.byte.3.offset67 = add i64 %shadow.ptr.int59, 24, !dbg !39
  %shadow.byte.3.ptr68 = inttoptr i64 %shadow.byte.3.offset67 to ptr, !dbg !39
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr68, align 8, !dbg !39
  %shadow.byte.4.offset69 = add i64 %shadow.ptr.int59, 32, !dbg !39
  %shadow.byte.4.ptr70 = inttoptr i64 %shadow.byte.4.offset69 to ptr, !dbg !39
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr70, align 8, !dbg !39
  %shadow.byte.5.offset71 = add i64 %shadow.ptr.int59, 40, !dbg !39
  %shadow.byte.5.ptr72 = inttoptr i64 %shadow.byte.5.offset71 to ptr, !dbg !39
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr72, align 8, !dbg !39
  %shadow.byte.6.offset73 = add i64 %shadow.ptr.int59, 48, !dbg !39
  %shadow.byte.6.ptr74 = inttoptr i64 %shadow.byte.6.offset73 to ptr, !dbg !39
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr74, align 8, !dbg !39
  %shadow.byte.7.offset75 = add i64 %shadow.ptr.int59, 56, !dbg !39
  %shadow.byte.7.ptr76 = inttoptr i64 %shadow.byte.7.offset75 to ptr, !dbg !39
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr76, align 8, !dbg !39
  br label %366, !dbg !39

365:                                              ; preds = %325
  call void @__tysan_check(ptr %324, i32 8, ptr @__tysan_v1_double_o_0, i32 1), !dbg !39
  br label %366, !dbg !39

366:                                              ; preds = %365, %364
  br label %412, !dbg !39

367:                                              ; preds = %323
  %368 = add i64 %shadow.ptr.int59, 8, !dbg !39
  %369 = inttoptr i64 %368 to ptr, !dbg !39
  %370 = load ptr, ptr %369, align 8, !dbg !39
  %371 = ptrtoint ptr %370 to i64, !dbg !39
  %372 = icmp sge i64 %371, 0, !dbg !39
  %373 = or i1 false, %372, !dbg !39
  %374 = add i64 %shadow.ptr.int59, 16, !dbg !39
  %375 = inttoptr i64 %374 to ptr, !dbg !39
  %376 = load ptr, ptr %375, align 8, !dbg !39
  %377 = ptrtoint ptr %376 to i64, !dbg !39
  %378 = icmp sge i64 %377, 0, !dbg !39
  %379 = or i1 %373, %378, !dbg !39
  %380 = add i64 %shadow.ptr.int59, 24, !dbg !39
  %381 = inttoptr i64 %380 to ptr, !dbg !39
  %382 = load ptr, ptr %381, align 8, !dbg !39
  %383 = ptrtoint ptr %382 to i64, !dbg !39
  %384 = icmp sge i64 %383, 0, !dbg !39
  %385 = or i1 %379, %384, !dbg !39
  %386 = add i64 %shadow.ptr.int59, 32, !dbg !39
  %387 = inttoptr i64 %386 to ptr, !dbg !39
  %388 = load ptr, ptr %387, align 8, !dbg !39
  %389 = ptrtoint ptr %388 to i64, !dbg !39
  %390 = icmp sge i64 %389, 0, !dbg !39
  %391 = or i1 %385, %390, !dbg !39
  %392 = add i64 %shadow.ptr.int59, 40, !dbg !39
  %393 = inttoptr i64 %392 to ptr, !dbg !39
  %394 = load ptr, ptr %393, align 8, !dbg !39
  %395 = ptrtoint ptr %394 to i64, !dbg !39
  %396 = icmp sge i64 %395, 0, !dbg !39
  %397 = or i1 %391, %396, !dbg !39
  %398 = add i64 %shadow.ptr.int59, 48, !dbg !39
  %399 = inttoptr i64 %398 to ptr, !dbg !39
  %400 = load ptr, ptr %399, align 8, !dbg !39
  %401 = ptrtoint ptr %400 to i64, !dbg !39
  %402 = icmp sge i64 %401, 0, !dbg !39
  %403 = or i1 %397, %402, !dbg !39
  %404 = add i64 %shadow.ptr.int59, 56, !dbg !39
  %405 = inttoptr i64 %404 to ptr, !dbg !39
  %406 = load ptr, ptr %405, align 8, !dbg !39
  %407 = ptrtoint ptr %406 to i64, !dbg !39
  %408 = icmp sge i64 %407, 0, !dbg !39
  %409 = or i1 %403, %408, !dbg !39
  br i1 %409, label %410, label %411, !dbg !39, !prof !20

410:                                              ; preds = %367
  call void @__tysan_check(ptr %324, i32 8, ptr @__tysan_v1_double_o_0, i32 1), !dbg !39
  br label %411, !dbg !39

411:                                              ; preds = %367, %410
  br label %412, !dbg !39

412:                                              ; preds = %411, %366
  %413 = load double, ptr %324, align 8, !dbg !39, !tbaa !40
  %414 = fadd double %413, 2.000000e+00, !dbg !39
  %app.ptr.int77 = ptrtoint ptr %324 to i64, !dbg !39
  %app.ptr.masked78 = and i64 %app.ptr.int77, %app.mem.mask, !dbg !39
  %app.ptr.shifted79 = shl i64 %app.ptr.masked78, 3, !dbg !39
  %shadow.ptr.int80 = add i64 %app.ptr.shifted79, %shadow.base, !dbg !39
  %shadow.ptr81 = inttoptr i64 %shadow.ptr.int80 to ptr, !dbg !39
  %shadow.desc82 = load ptr, ptr %shadow.ptr81, align 8, !dbg !39
  %bad.desc83 = icmp ne ptr %shadow.desc82, null, !dbg !39
  br i1 %bad.desc83, label %415, label %457, !dbg !39, !prof !20

415:                                              ; preds = %412
  %416 = icmp eq ptr %shadow.desc82, null, !dbg !39
  br i1 %416, label %417, label %455, !dbg !39

417:                                              ; preds = %415
  %418 = add i64 %shadow.ptr.int80, 8, !dbg !39
  %419 = inttoptr i64 %418 to ptr, !dbg !39
  %420 = load ptr, ptr %419, align 8, !dbg !39
  %421 = icmp ne ptr %420, null, !dbg !39
  %422 = or i1 false, %421, !dbg !39
  %423 = add i64 %shadow.ptr.int80, 16, !dbg !39
  %424 = inttoptr i64 %423 to ptr, !dbg !39
  %425 = load ptr, ptr %424, align 8, !dbg !39
  %426 = icmp ne ptr %425, null, !dbg !39
  %427 = or i1 %422, %426, !dbg !39
  %428 = add i64 %shadow.ptr.int80, 24, !dbg !39
  %429 = inttoptr i64 %428 to ptr, !dbg !39
  %430 = load ptr, ptr %429, align 8, !dbg !39
  %431 = icmp ne ptr %430, null, !dbg !39
  %432 = or i1 %427, %431, !dbg !39
  %433 = add i64 %shadow.ptr.int80, 32, !dbg !39
  %434 = inttoptr i64 %433 to ptr, !dbg !39
  %435 = load ptr, ptr %434, align 8, !dbg !39
  %436 = icmp ne ptr %435, null, !dbg !39
  %437 = or i1 %432, %436, !dbg !39
  %438 = add i64 %shadow.ptr.int80, 40, !dbg !39
  %439 = inttoptr i64 %438 to ptr, !dbg !39
  %440 = load ptr, ptr %439, align 8, !dbg !39
  %441 = icmp ne ptr %440, null, !dbg !39
  %442 = or i1 %437, %441, !dbg !39
  %443 = add i64 %shadow.ptr.int80, 48, !dbg !39
  %444 = inttoptr i64 %443 to ptr, !dbg !39
  %445 = load ptr, ptr %444, align 8, !dbg !39
  %446 = icmp ne ptr %445, null, !dbg !39
  %447 = or i1 %442, %446, !dbg !39
  %448 = add i64 %shadow.ptr.int80, 56, !dbg !39
  %449 = inttoptr i64 %448 to ptr, !dbg !39
  %450 = load ptr, ptr %449, align 8, !dbg !39
  %451 = icmp ne ptr %450, null, !dbg !39
  %452 = or i1 %447, %451, !dbg !39
  br i1 %452, label %453, label %454, !dbg !39, !prof !20

453:                                              ; preds = %417
  call void @__tysan_check(ptr %324, i32 8, ptr null, i32 2), !dbg !39
  br label %454, !dbg !39

454:                                              ; preds = %417, %453
  store ptr null, ptr %shadow.ptr81, align 8, !dbg !39
  %shadow.byte.1.offset84 = add i64 %shadow.ptr.int80, 8, !dbg !39
  %shadow.byte.1.ptr85 = inttoptr i64 %shadow.byte.1.offset84 to ptr, !dbg !39
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr85, align 8, !dbg !39
  %shadow.byte.2.offset86 = add i64 %shadow.ptr.int80, 16, !dbg !39
  %shadow.byte.2.ptr87 = inttoptr i64 %shadow.byte.2.offset86 to ptr, !dbg !39
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr87, align 8, !dbg !39
  %shadow.byte.3.offset88 = add i64 %shadow.ptr.int80, 24, !dbg !39
  %shadow.byte.3.ptr89 = inttoptr i64 %shadow.byte.3.offset88 to ptr, !dbg !39
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr89, align 8, !dbg !39
  %shadow.byte.4.offset90 = add i64 %shadow.ptr.int80, 32, !dbg !39
  %shadow.byte.4.ptr91 = inttoptr i64 %shadow.byte.4.offset90 to ptr, !dbg !39
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr91, align 8, !dbg !39
  %shadow.byte.5.offset92 = add i64 %shadow.ptr.int80, 40, !dbg !39
  %shadow.byte.5.ptr93 = inttoptr i64 %shadow.byte.5.offset92 to ptr, !dbg !39
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr93, align 8, !dbg !39
  %shadow.byte.6.offset94 = add i64 %shadow.ptr.int80, 48, !dbg !39
  %shadow.byte.6.ptr95 = inttoptr i64 %shadow.byte.6.offset94 to ptr, !dbg !39
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr95, align 8, !dbg !39
  %shadow.byte.7.offset96 = add i64 %shadow.ptr.int80, 56, !dbg !39
  %shadow.byte.7.ptr97 = inttoptr i64 %shadow.byte.7.offset96 to ptr, !dbg !39
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr97, align 8, !dbg !39
  br label %456, !dbg !39

455:                                              ; preds = %415
  call void @__tysan_check(ptr %324, i32 8, ptr null, i32 2), !dbg !39
  br label %456, !dbg !39

456:                                              ; preds = %455, %454
  br label %502, !dbg !39

457:                                              ; preds = %412
  %458 = add i64 %shadow.ptr.int80, 8, !dbg !39
  %459 = inttoptr i64 %458 to ptr, !dbg !39
  %460 = load ptr, ptr %459, align 8, !dbg !39
  %461 = ptrtoint ptr %460 to i64, !dbg !39
  %462 = icmp sge i64 %461, 0, !dbg !39
  %463 = or i1 false, %462, !dbg !39
  %464 = add i64 %shadow.ptr.int80, 16, !dbg !39
  %465 = inttoptr i64 %464 to ptr, !dbg !39
  %466 = load ptr, ptr %465, align 8, !dbg !39
  %467 = ptrtoint ptr %466 to i64, !dbg !39
  %468 = icmp sge i64 %467, 0, !dbg !39
  %469 = or i1 %463, %468, !dbg !39
  %470 = add i64 %shadow.ptr.int80, 24, !dbg !39
  %471 = inttoptr i64 %470 to ptr, !dbg !39
  %472 = load ptr, ptr %471, align 8, !dbg !39
  %473 = ptrtoint ptr %472 to i64, !dbg !39
  %474 = icmp sge i64 %473, 0, !dbg !39
  %475 = or i1 %469, %474, !dbg !39
  %476 = add i64 %shadow.ptr.int80, 32, !dbg !39
  %477 = inttoptr i64 %476 to ptr, !dbg !39
  %478 = load ptr, ptr %477, align 8, !dbg !39
  %479 = ptrtoint ptr %478 to i64, !dbg !39
  %480 = icmp sge i64 %479, 0, !dbg !39
  %481 = or i1 %475, %480, !dbg !39
  %482 = add i64 %shadow.ptr.int80, 40, !dbg !39
  %483 = inttoptr i64 %482 to ptr, !dbg !39
  %484 = load ptr, ptr %483, align 8, !dbg !39
  %485 = ptrtoint ptr %484 to i64, !dbg !39
  %486 = icmp sge i64 %485, 0, !dbg !39
  %487 = or i1 %481, %486, !dbg !39
  %488 = add i64 %shadow.ptr.int80, 48, !dbg !39
  %489 = inttoptr i64 %488 to ptr, !dbg !39
  %490 = load ptr, ptr %489, align 8, !dbg !39
  %491 = ptrtoint ptr %490 to i64, !dbg !39
  %492 = icmp sge i64 %491, 0, !dbg !39
  %493 = or i1 %487, %492, !dbg !39
  %494 = add i64 %shadow.ptr.int80, 56, !dbg !39
  %495 = inttoptr i64 %494 to ptr, !dbg !39
  %496 = load ptr, ptr %495, align 8, !dbg !39
  %497 = ptrtoint ptr %496 to i64, !dbg !39
  %498 = icmp sge i64 %497, 0, !dbg !39
  %499 = or i1 %493, %498, !dbg !39
  br i1 %499, label %500, label %501, !dbg !39, !prof !20

500:                                              ; preds = %457
  call void @__tysan_check(ptr %324, i32 8, ptr null, i32 2), !dbg !39
  br label %501, !dbg !39

501:                                              ; preds = %457, %500
  br label %502, !dbg !39

502:                                              ; preds = %501, %456
  store double %414, ptr %324, align 8, !dbg !39
  %app.ptr.int98 = ptrtoint ptr %result to i64, !dbg !42
  %app.ptr.masked99 = and i64 %app.ptr.int98, %app.mem.mask, !dbg !42
  %app.ptr.shifted100 = shl i64 %app.ptr.masked99, 3, !dbg !42
  %shadow.ptr.int101 = add i64 %app.ptr.shifted100, %shadow.base, !dbg !42
  %shadow.ptr102 = inttoptr i64 %shadow.ptr.int101 to ptr, !dbg !42
  %shadow.desc103 = load ptr, ptr %shadow.ptr102, align 8, !dbg !42
  %bad.desc104 = icmp ne ptr %shadow.desc103, null, !dbg !42
  br i1 %bad.desc104, label %503, label %525, !dbg !42, !prof !20

503:                                              ; preds = %502
  %504 = icmp eq ptr %shadow.desc103, null, !dbg !42
  br i1 %504, label %505, label %523, !dbg !42

505:                                              ; preds = %503
  %506 = add i64 %shadow.ptr.int101, 8, !dbg !42
  %507 = inttoptr i64 %506 to ptr, !dbg !42
  %508 = load ptr, ptr %507, align 8, !dbg !42
  %509 = icmp ne ptr %508, null, !dbg !42
  %510 = or i1 false, %509, !dbg !42
  %511 = add i64 %shadow.ptr.int101, 16, !dbg !42
  %512 = inttoptr i64 %511 to ptr, !dbg !42
  %513 = load ptr, ptr %512, align 8, !dbg !42
  %514 = icmp ne ptr %513, null, !dbg !42
  %515 = or i1 %510, %514, !dbg !42
  %516 = add i64 %shadow.ptr.int101, 24, !dbg !42
  %517 = inttoptr i64 %516 to ptr, !dbg !42
  %518 = load ptr, ptr %517, align 8, !dbg !42
  %519 = icmp ne ptr %518, null, !dbg !42
  %520 = or i1 %515, %519, !dbg !42
  br i1 %520, label %521, label %522, !dbg !42, !prof !20

521:                                              ; preds = %505
  call void @__tysan_check(ptr %result, i32 4, ptr null, i32 1), !dbg !42
  br label %522, !dbg !42

522:                                              ; preds = %505, %521
  store ptr null, ptr %shadow.ptr102, align 8, !dbg !42
  %shadow.byte.1.offset105 = add i64 %shadow.ptr.int101, 8, !dbg !42
  %shadow.byte.1.ptr106 = inttoptr i64 %shadow.byte.1.offset105 to ptr, !dbg !42
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr106, align 8, !dbg !42
  %shadow.byte.2.offset107 = add i64 %shadow.ptr.int101, 16, !dbg !42
  %shadow.byte.2.ptr108 = inttoptr i64 %shadow.byte.2.offset107 to ptr, !dbg !42
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr108, align 8, !dbg !42
  %shadow.byte.3.offset109 = add i64 %shadow.ptr.int101, 24, !dbg !42
  %shadow.byte.3.ptr110 = inttoptr i64 %shadow.byte.3.offset109 to ptr, !dbg !42
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr110, align 8, !dbg !42
  br label %524, !dbg !42

523:                                              ; preds = %503
  call void @__tysan_check(ptr %result, i32 4, ptr null, i32 1), !dbg !42
  br label %524, !dbg !42

524:                                              ; preds = %523, %522
  br label %546, !dbg !42

525:                                              ; preds = %502
  %526 = add i64 %shadow.ptr.int101, 8, !dbg !42
  %527 = inttoptr i64 %526 to ptr, !dbg !42
  %528 = load ptr, ptr %527, align 8, !dbg !42
  %529 = ptrtoint ptr %528 to i64, !dbg !42
  %530 = icmp sge i64 %529, 0, !dbg !42
  %531 = or i1 false, %530, !dbg !42
  %532 = add i64 %shadow.ptr.int101, 16, !dbg !42
  %533 = inttoptr i64 %532 to ptr, !dbg !42
  %534 = load ptr, ptr %533, align 8, !dbg !42
  %535 = ptrtoint ptr %534 to i64, !dbg !42
  %536 = icmp sge i64 %535, 0, !dbg !42
  %537 = or i1 %531, %536, !dbg !42
  %538 = add i64 %shadow.ptr.int101, 24, !dbg !42
  %539 = inttoptr i64 %538 to ptr, !dbg !42
  %540 = load ptr, ptr %539, align 8, !dbg !42
  %541 = ptrtoint ptr %540 to i64, !dbg !42
  %542 = icmp sge i64 %541, 0, !dbg !42
  %543 = or i1 %537, %542, !dbg !42
  br i1 %543, label %544, label %545, !dbg !42, !prof !20

544:                                              ; preds = %525
  call void @__tysan_check(ptr %result, i32 4, ptr null, i32 1), !dbg !42
  br label %545, !dbg !42

545:                                              ; preds = %525, %544
  br label %546, !dbg !42

546:                                              ; preds = %545, %524
  %547 = load i32, ptr %result, align 4, !dbg !42
  ret i32 %547, !dbg !42
}

declare void @__tysan_init()

; Function Attrs: nounwind uwtable
define internal void @tysan.module_ctor() #1 {
  call void @__tysan_init()
  ret void
}

; Function Attrs: nounwind
declare void @__tysan_check(ptr, i32, ptr, i32) #2

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #3

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_type uwtable }
attributes #1 = { nounwind uwtable "frame-pointer"="all" }
attributes #2 = { nounwind }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: write) }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!6}
!llvm.dbg.cu = !{!7}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!7 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !8, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!8 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/instrumentation/success-tysan-and-dbg-info/source.spice", directory: "./test-files/irgenerator/instrumentation/success-tysan-and-dbg-info")
!9 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !10, file: !10, line: 3, type: !11, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7, retainedNodes: !14)
!10 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-tysan-and-dbg-info")
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !{i64 -5436590890822487957, !"int"}
!16 = !{i64 6042226155786013045, !"long"}
!17 = !DILocation(line: 3, column: 1, scope: !9)
!18 = !{i64 7516313782037075379, !"double*"}
!19 = !DILocalVariable(name: "result", scope: !9, file: !10, line: 3, type: !13)
!20 = !{!"branch_weights", i32 1, i32 100000}
!21 = !{!22, !22, i64 0}
!22 = !{!"int", !23, i64 0}
!23 = !{!"omnipotent byte", !24, i64 0}
!24 = !{!"Simple Spice TBAA"}
!25 = !DILocation(line: 4, column: 14, scope: !9)
!26 = !{!27, !27, i64 0}
!27 = !{!"long", !23, i64 0}
!28 = !DILocalVariable(name: "l", scope: !9, file: !10, line: 4, type: !29)
!29 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!30 = !DILocation(line: 4, column: 5, scope: !9)
!31 = !DILocation(line: 6, column: 37, scope: !32)
!32 = distinct !DILexicalBlock(scope: !9, file: !10, line: 5, column: 5)
!33 = !{!34, !34, i64 0}
!34 = !{!"double*", !23, i64 0}
!35 = !DILocalVariable(name: "ptr", scope: !32, file: !10, line: 6, type: !36)
!36 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !37, size: 64)
!37 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!38 = !DILocation(line: 6, column: 8, scope: !32)
!39 = !DILocation(line: 7, column: 9, scope: !32)
!40 = !{!41, !41, i64 0}
!41 = !{!"double", !23, i64 0}
!42 = !DILocation(line: 9, column: 1, scope: !9)
