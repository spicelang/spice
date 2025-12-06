; ModuleID = 'source.spice'
source_filename = "source.spice"

$__tysan_v1_Simple_20Spice_20TBAA = comdat any

$__tysan_v1_omnipotent_20byte = comdat any

$__tysan_v1_int = comdat any

$__tysan_v1_int_o_0 = comdat any

$__tysan_v1_long = comdat any

$__tysan_v1_long_o_0 = comdat any

$__tysan_v1_double_2a = comdat any

$__tysan_v1_double_2a_o_0 = comdat any

$__tysan_v1_double = comdat any

$__tysan_v1_double_o_0 = comdat any

@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @tysan.module_ctor, ptr null }]
@__tysan_v1_Simple_20Spice_20TBAA = linkonce_odr constant { i64, i64, [18 x i8] } { i64 2, i64 0, [18 x i8] c"Simple Spice TBAA\00" }, comdat
@__tysan_v1_omnipotent_20byte = linkonce_odr constant { i64, i64, ptr, i64, [16 x i8] } { i64 2, i64 1, ptr @__tysan_v1_Simple_20Spice_20TBAA, i64 0, [16 x i8] c"omnipotent byte\00" }, comdat
@__tysan_v1_int = linkonce_odr constant { i64, i64, ptr, i64, [4 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [4 x i8] c"int\00" }, comdat
@__tysan_v1_int_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_int, ptr @__tysan_v1_int, i64 0 }, comdat
@__tysan_v1_long = linkonce_odr constant { i64, i64, ptr, i64, [5 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [5 x i8] c"long\00" }, comdat
@__tysan_v1_long_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_long, ptr @__tysan_v1_long, i64 0 }, comdat
@__tysan_v1_double_2a = linkonce_odr constant { i64, i64, ptr, i64, [8 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [8 x i8] c"double*\00" }, comdat
@__tysan_v1_double_2a_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_double_2a, ptr @__tysan_v1_double_2a, i64 0 }, comdat
@__tysan_v1_double = linkonce_odr constant { i64, i64, ptr, i64, [7 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [7 x i8] c"double\00" }, comdat
@__tysan_v1_double_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_double, ptr @__tysan_v1_double, i64 0 }, comdat
@llvm.used = appending global [11 x ptr] [ptr @tysan.module_ctor, ptr @__tysan_v1_Simple_20Spice_20TBAA, ptr @__tysan_v1_omnipotent_20byte, ptr @__tysan_v1_int, ptr @__tysan_v1_int_o_0, ptr @__tysan_v1_long, ptr @__tysan_v1_long_o_0, ptr @__tysan_v1_double_2a, ptr @__tysan_v1_double_2a_o_0, ptr @__tysan_v1_double, ptr @__tysan_v1_double_o_0], section "llvm.metadata"
@__tysan_shadow_memory_address = external global i64
@__tysan_app_memory_mask = external global i64

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_type uwtable
define dso_local noundef i32 @main() #0 {
  %app.mem.mask = load i64, ptr @__tysan_app_memory_mask, align 8
  %shadow.base = load i64, ptr @__tysan_shadow_memory_address, align 8
  %result = alloca i32, align 4, !type !5
  %1 = ptrtoint ptr %result to i64
  %2 = and i64 %1, %app.mem.mask
  %3 = shl i64 %2, 3
  %4 = add i64 %3, %shadow.base
  %5 = inttoptr i64 %4 to ptr
  call void @llvm.memset.p0.i64(ptr align 8 %5, i8 0, i64 32, i1 false)
  %l = alloca i64, align 8, !type !6
  %6 = ptrtoint ptr %l to i64
  %7 = and i64 %6, %app.mem.mask
  %8 = shl i64 %7, 3
  %9 = add i64 %8, %shadow.base
  %10 = inttoptr i64 %9 to ptr
  call void @llvm.memset.p0.i64(ptr align 8 %10, i8 0, i64 64, i1 false)
  %ptr = alloca ptr, align 8, !type !7
  %11 = ptrtoint ptr %ptr to i64
  %12 = and i64 %11, %app.mem.mask
  %13 = shl i64 %12, 3
  %14 = add i64 %13, %shadow.base
  %15 = inttoptr i64 %14 to ptr
  call void @llvm.memset.p0.i64(ptr align 8 %15, i8 0, i64 64, i1 false)
  %app.ptr.int = ptrtoint ptr %result to i64
  %app.ptr.masked = and i64 %app.ptr.int, %app.mem.mask
  %app.ptr.shifted = shl i64 %app.ptr.masked, 3
  %shadow.ptr.int = add i64 %app.ptr.shifted, %shadow.base
  %shadow.ptr = inttoptr i64 %shadow.ptr.int to ptr
  %shadow.desc = load ptr, ptr %shadow.ptr, align 8
  %bad.desc = icmp ne ptr %shadow.desc, @__tysan_v1_int_o_0
  br i1 %bad.desc, label %16, label %38, !prof !8

16:                                               ; preds = %0
  %17 = icmp eq ptr %shadow.desc, null
  br i1 %17, label %18, label %36

18:                                               ; preds = %16
  %19 = add i64 %shadow.ptr.int, 8
  %20 = inttoptr i64 %19 to ptr
  %21 = load ptr, ptr %20, align 8
  %22 = icmp ne ptr %21, null
  %23 = or i1 false, %22
  %24 = add i64 %shadow.ptr.int, 16
  %25 = inttoptr i64 %24 to ptr
  %26 = load ptr, ptr %25, align 8
  %27 = icmp ne ptr %26, null
  %28 = or i1 %23, %27
  %29 = add i64 %shadow.ptr.int, 24
  %30 = inttoptr i64 %29 to ptr
  %31 = load ptr, ptr %30, align 8
  %32 = icmp ne ptr %31, null
  %33 = or i1 %28, %32
  br i1 %33, label %34, label %35, !prof !8

34:                                               ; preds = %18
  call void @__tysan_check(ptr %result, i32 4, ptr @__tysan_v1_int_o_0, i32 2)
  br label %35

35:                                               ; preds = %18, %34
  store ptr @__tysan_v1_int_o_0, ptr %shadow.ptr, align 8
  %shadow.byte.1.offset = add i64 %shadow.ptr.int, 8
  %shadow.byte.1.ptr = inttoptr i64 %shadow.byte.1.offset to ptr
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr, align 8
  %shadow.byte.2.offset = add i64 %shadow.ptr.int, 16
  %shadow.byte.2.ptr = inttoptr i64 %shadow.byte.2.offset to ptr
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr, align 8
  %shadow.byte.3.offset = add i64 %shadow.ptr.int, 24
  %shadow.byte.3.ptr = inttoptr i64 %shadow.byte.3.offset to ptr
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr, align 8
  br label %37

36:                                               ; preds = %16
  call void @__tysan_check(ptr %result, i32 4, ptr @__tysan_v1_int_o_0, i32 2)
  br label %37

37:                                               ; preds = %36, %35
  br label %59

38:                                               ; preds = %0
  %39 = add i64 %shadow.ptr.int, 8
  %40 = inttoptr i64 %39 to ptr
  %41 = load ptr, ptr %40, align 8
  %42 = ptrtoint ptr %41 to i64
  %43 = icmp sge i64 %42, 0
  %44 = or i1 false, %43
  %45 = add i64 %shadow.ptr.int, 16
  %46 = inttoptr i64 %45 to ptr
  %47 = load ptr, ptr %46, align 8
  %48 = ptrtoint ptr %47 to i64
  %49 = icmp sge i64 %48, 0
  %50 = or i1 %44, %49
  %51 = add i64 %shadow.ptr.int, 24
  %52 = inttoptr i64 %51 to ptr
  %53 = load ptr, ptr %52, align 8
  %54 = ptrtoint ptr %53 to i64
  %55 = icmp sge i64 %54, 0
  %56 = or i1 %50, %55
  br i1 %56, label %57, label %58, !prof !8

57:                                               ; preds = %38
  call void @__tysan_check(ptr %result, i32 4, ptr @__tysan_v1_int_o_0, i32 2)
  br label %58

58:                                               ; preds = %38, %57
  br label %59

59:                                               ; preds = %58, %37
  store i32 0, ptr %result, align 4, !tbaa !9
  %app.ptr.int1 = ptrtoint ptr %l to i64
  %app.ptr.masked2 = and i64 %app.ptr.int1, %app.mem.mask
  %app.ptr.shifted3 = shl i64 %app.ptr.masked2, 3
  %shadow.ptr.int4 = add i64 %app.ptr.shifted3, %shadow.base
  %shadow.ptr5 = inttoptr i64 %shadow.ptr.int4 to ptr
  %shadow.desc6 = load ptr, ptr %shadow.ptr5, align 8
  %bad.desc7 = icmp ne ptr %shadow.desc6, @__tysan_v1_long_o_0
  br i1 %bad.desc7, label %60, label %102, !prof !8

60:                                               ; preds = %59
  %61 = icmp eq ptr %shadow.desc6, null
  br i1 %61, label %62, label %100

62:                                               ; preds = %60
  %63 = add i64 %shadow.ptr.int4, 8
  %64 = inttoptr i64 %63 to ptr
  %65 = load ptr, ptr %64, align 8
  %66 = icmp ne ptr %65, null
  %67 = or i1 false, %66
  %68 = add i64 %shadow.ptr.int4, 16
  %69 = inttoptr i64 %68 to ptr
  %70 = load ptr, ptr %69, align 8
  %71 = icmp ne ptr %70, null
  %72 = or i1 %67, %71
  %73 = add i64 %shadow.ptr.int4, 24
  %74 = inttoptr i64 %73 to ptr
  %75 = load ptr, ptr %74, align 8
  %76 = icmp ne ptr %75, null
  %77 = or i1 %72, %76
  %78 = add i64 %shadow.ptr.int4, 32
  %79 = inttoptr i64 %78 to ptr
  %80 = load ptr, ptr %79, align 8
  %81 = icmp ne ptr %80, null
  %82 = or i1 %77, %81
  %83 = add i64 %shadow.ptr.int4, 40
  %84 = inttoptr i64 %83 to ptr
  %85 = load ptr, ptr %84, align 8
  %86 = icmp ne ptr %85, null
  %87 = or i1 %82, %86
  %88 = add i64 %shadow.ptr.int4, 48
  %89 = inttoptr i64 %88 to ptr
  %90 = load ptr, ptr %89, align 8
  %91 = icmp ne ptr %90, null
  %92 = or i1 %87, %91
  %93 = add i64 %shadow.ptr.int4, 56
  %94 = inttoptr i64 %93 to ptr
  %95 = load ptr, ptr %94, align 8
  %96 = icmp ne ptr %95, null
  %97 = or i1 %92, %96
  br i1 %97, label %98, label %99, !prof !8

98:                                               ; preds = %62
  call void @__tysan_check(ptr %l, i32 8, ptr @__tysan_v1_long_o_0, i32 2)
  br label %99

99:                                               ; preds = %62, %98
  store ptr @__tysan_v1_long_o_0, ptr %shadow.ptr5, align 8
  %shadow.byte.1.offset8 = add i64 %shadow.ptr.int4, 8
  %shadow.byte.1.ptr9 = inttoptr i64 %shadow.byte.1.offset8 to ptr
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr9, align 8
  %shadow.byte.2.offset10 = add i64 %shadow.ptr.int4, 16
  %shadow.byte.2.ptr11 = inttoptr i64 %shadow.byte.2.offset10 to ptr
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr11, align 8
  %shadow.byte.3.offset12 = add i64 %shadow.ptr.int4, 24
  %shadow.byte.3.ptr13 = inttoptr i64 %shadow.byte.3.offset12 to ptr
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr13, align 8
  %shadow.byte.4.offset = add i64 %shadow.ptr.int4, 32
  %shadow.byte.4.ptr = inttoptr i64 %shadow.byte.4.offset to ptr
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr, align 8
  %shadow.byte.5.offset = add i64 %shadow.ptr.int4, 40
  %shadow.byte.5.ptr = inttoptr i64 %shadow.byte.5.offset to ptr
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr, align 8
  %shadow.byte.6.offset = add i64 %shadow.ptr.int4, 48
  %shadow.byte.6.ptr = inttoptr i64 %shadow.byte.6.offset to ptr
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr, align 8
  %shadow.byte.7.offset = add i64 %shadow.ptr.int4, 56
  %shadow.byte.7.ptr = inttoptr i64 %shadow.byte.7.offset to ptr
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr, align 8
  br label %101

100:                                              ; preds = %60
  call void @__tysan_check(ptr %l, i32 8, ptr @__tysan_v1_long_o_0, i32 2)
  br label %101

101:                                              ; preds = %100, %99
  br label %147

102:                                              ; preds = %59
  %103 = add i64 %shadow.ptr.int4, 8
  %104 = inttoptr i64 %103 to ptr
  %105 = load ptr, ptr %104, align 8
  %106 = ptrtoint ptr %105 to i64
  %107 = icmp sge i64 %106, 0
  %108 = or i1 false, %107
  %109 = add i64 %shadow.ptr.int4, 16
  %110 = inttoptr i64 %109 to ptr
  %111 = load ptr, ptr %110, align 8
  %112 = ptrtoint ptr %111 to i64
  %113 = icmp sge i64 %112, 0
  %114 = or i1 %108, %113
  %115 = add i64 %shadow.ptr.int4, 24
  %116 = inttoptr i64 %115 to ptr
  %117 = load ptr, ptr %116, align 8
  %118 = ptrtoint ptr %117 to i64
  %119 = icmp sge i64 %118, 0
  %120 = or i1 %114, %119
  %121 = add i64 %shadow.ptr.int4, 32
  %122 = inttoptr i64 %121 to ptr
  %123 = load ptr, ptr %122, align 8
  %124 = ptrtoint ptr %123 to i64
  %125 = icmp sge i64 %124, 0
  %126 = or i1 %120, %125
  %127 = add i64 %shadow.ptr.int4, 40
  %128 = inttoptr i64 %127 to ptr
  %129 = load ptr, ptr %128, align 8
  %130 = ptrtoint ptr %129 to i64
  %131 = icmp sge i64 %130, 0
  %132 = or i1 %126, %131
  %133 = add i64 %shadow.ptr.int4, 48
  %134 = inttoptr i64 %133 to ptr
  %135 = load ptr, ptr %134, align 8
  %136 = ptrtoint ptr %135 to i64
  %137 = icmp sge i64 %136, 0
  %138 = or i1 %132, %137
  %139 = add i64 %shadow.ptr.int4, 56
  %140 = inttoptr i64 %139 to ptr
  %141 = load ptr, ptr %140, align 8
  %142 = ptrtoint ptr %141 to i64
  %143 = icmp sge i64 %142, 0
  %144 = or i1 %138, %143
  br i1 %144, label %145, label %146, !prof !8

145:                                              ; preds = %102
  call void @__tysan_check(ptr %l, i32 8, ptr @__tysan_v1_long_o_0, i32 2)
  br label %146

146:                                              ; preds = %102, %145
  br label %147

147:                                              ; preds = %146, %101
  store i64 100, ptr %l, align 8, !tbaa !13
  %app.ptr.int14 = ptrtoint ptr %ptr to i64
  %app.ptr.masked15 = and i64 %app.ptr.int14, %app.mem.mask
  %app.ptr.shifted16 = shl i64 %app.ptr.masked15, 3
  %shadow.ptr.int17 = add i64 %app.ptr.shifted16, %shadow.base
  %shadow.ptr18 = inttoptr i64 %shadow.ptr.int17 to ptr
  %shadow.desc19 = load ptr, ptr %shadow.ptr18, align 8
  %bad.desc20 = icmp ne ptr %shadow.desc19, @__tysan_v1_double_2a_o_0
  br i1 %bad.desc20, label %148, label %190, !prof !8

148:                                              ; preds = %147
  %149 = icmp eq ptr %shadow.desc19, null
  br i1 %149, label %150, label %188

150:                                              ; preds = %148
  %151 = add i64 %shadow.ptr.int17, 8
  %152 = inttoptr i64 %151 to ptr
  %153 = load ptr, ptr %152, align 8
  %154 = icmp ne ptr %153, null
  %155 = or i1 false, %154
  %156 = add i64 %shadow.ptr.int17, 16
  %157 = inttoptr i64 %156 to ptr
  %158 = load ptr, ptr %157, align 8
  %159 = icmp ne ptr %158, null
  %160 = or i1 %155, %159
  %161 = add i64 %shadow.ptr.int17, 24
  %162 = inttoptr i64 %161 to ptr
  %163 = load ptr, ptr %162, align 8
  %164 = icmp ne ptr %163, null
  %165 = or i1 %160, %164
  %166 = add i64 %shadow.ptr.int17, 32
  %167 = inttoptr i64 %166 to ptr
  %168 = load ptr, ptr %167, align 8
  %169 = icmp ne ptr %168, null
  %170 = or i1 %165, %169
  %171 = add i64 %shadow.ptr.int17, 40
  %172 = inttoptr i64 %171 to ptr
  %173 = load ptr, ptr %172, align 8
  %174 = icmp ne ptr %173, null
  %175 = or i1 %170, %174
  %176 = add i64 %shadow.ptr.int17, 48
  %177 = inttoptr i64 %176 to ptr
  %178 = load ptr, ptr %177, align 8
  %179 = icmp ne ptr %178, null
  %180 = or i1 %175, %179
  %181 = add i64 %shadow.ptr.int17, 56
  %182 = inttoptr i64 %181 to ptr
  %183 = load ptr, ptr %182, align 8
  %184 = icmp ne ptr %183, null
  %185 = or i1 %180, %184
  br i1 %185, label %186, label %187, !prof !8

186:                                              ; preds = %150
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 2)
  br label %187

187:                                              ; preds = %150, %186
  store ptr @__tysan_v1_double_2a_o_0, ptr %shadow.ptr18, align 8
  %shadow.byte.1.offset21 = add i64 %shadow.ptr.int17, 8
  %shadow.byte.1.ptr22 = inttoptr i64 %shadow.byte.1.offset21 to ptr
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr22, align 8
  %shadow.byte.2.offset23 = add i64 %shadow.ptr.int17, 16
  %shadow.byte.2.ptr24 = inttoptr i64 %shadow.byte.2.offset23 to ptr
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr24, align 8
  %shadow.byte.3.offset25 = add i64 %shadow.ptr.int17, 24
  %shadow.byte.3.ptr26 = inttoptr i64 %shadow.byte.3.offset25 to ptr
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr26, align 8
  %shadow.byte.4.offset27 = add i64 %shadow.ptr.int17, 32
  %shadow.byte.4.ptr28 = inttoptr i64 %shadow.byte.4.offset27 to ptr
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr28, align 8
  %shadow.byte.5.offset29 = add i64 %shadow.ptr.int17, 40
  %shadow.byte.5.ptr30 = inttoptr i64 %shadow.byte.5.offset29 to ptr
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr30, align 8
  %shadow.byte.6.offset31 = add i64 %shadow.ptr.int17, 48
  %shadow.byte.6.ptr32 = inttoptr i64 %shadow.byte.6.offset31 to ptr
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr32, align 8
  %shadow.byte.7.offset33 = add i64 %shadow.ptr.int17, 56
  %shadow.byte.7.ptr34 = inttoptr i64 %shadow.byte.7.offset33 to ptr
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr34, align 8
  br label %189

188:                                              ; preds = %148
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 2)
  br label %189

189:                                              ; preds = %188, %187
  br label %235

190:                                              ; preds = %147
  %191 = add i64 %shadow.ptr.int17, 8
  %192 = inttoptr i64 %191 to ptr
  %193 = load ptr, ptr %192, align 8
  %194 = ptrtoint ptr %193 to i64
  %195 = icmp sge i64 %194, 0
  %196 = or i1 false, %195
  %197 = add i64 %shadow.ptr.int17, 16
  %198 = inttoptr i64 %197 to ptr
  %199 = load ptr, ptr %198, align 8
  %200 = ptrtoint ptr %199 to i64
  %201 = icmp sge i64 %200, 0
  %202 = or i1 %196, %201
  %203 = add i64 %shadow.ptr.int17, 24
  %204 = inttoptr i64 %203 to ptr
  %205 = load ptr, ptr %204, align 8
  %206 = ptrtoint ptr %205 to i64
  %207 = icmp sge i64 %206, 0
  %208 = or i1 %202, %207
  %209 = add i64 %shadow.ptr.int17, 32
  %210 = inttoptr i64 %209 to ptr
  %211 = load ptr, ptr %210, align 8
  %212 = ptrtoint ptr %211 to i64
  %213 = icmp sge i64 %212, 0
  %214 = or i1 %208, %213
  %215 = add i64 %shadow.ptr.int17, 40
  %216 = inttoptr i64 %215 to ptr
  %217 = load ptr, ptr %216, align 8
  %218 = ptrtoint ptr %217 to i64
  %219 = icmp sge i64 %218, 0
  %220 = or i1 %214, %219
  %221 = add i64 %shadow.ptr.int17, 48
  %222 = inttoptr i64 %221 to ptr
  %223 = load ptr, ptr %222, align 8
  %224 = ptrtoint ptr %223 to i64
  %225 = icmp sge i64 %224, 0
  %226 = or i1 %220, %225
  %227 = add i64 %shadow.ptr.int17, 56
  %228 = inttoptr i64 %227 to ptr
  %229 = load ptr, ptr %228, align 8
  %230 = ptrtoint ptr %229 to i64
  %231 = icmp sge i64 %230, 0
  %232 = or i1 %226, %231
  br i1 %232, label %233, label %234, !prof !8

233:                                              ; preds = %190
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 2)
  br label %234

234:                                              ; preds = %190, %233
  br label %235

235:                                              ; preds = %234, %189
  store ptr %l, ptr %ptr, align 8, !tbaa !15
  %app.ptr.int35 = ptrtoint ptr %ptr to i64
  %app.ptr.masked36 = and i64 %app.ptr.int35, %app.mem.mask
  %app.ptr.shifted37 = shl i64 %app.ptr.masked36, 3
  %shadow.ptr.int38 = add i64 %app.ptr.shifted37, %shadow.base
  %shadow.ptr39 = inttoptr i64 %shadow.ptr.int38 to ptr
  %shadow.desc40 = load ptr, ptr %shadow.ptr39, align 8
  %bad.desc41 = icmp ne ptr %shadow.desc40, @__tysan_v1_double_2a_o_0
  br i1 %bad.desc41, label %236, label %278, !prof !8

236:                                              ; preds = %235
  %237 = icmp eq ptr %shadow.desc40, null
  br i1 %237, label %238, label %276

238:                                              ; preds = %236
  %239 = add i64 %shadow.ptr.int38, 8
  %240 = inttoptr i64 %239 to ptr
  %241 = load ptr, ptr %240, align 8
  %242 = icmp ne ptr %241, null
  %243 = or i1 false, %242
  %244 = add i64 %shadow.ptr.int38, 16
  %245 = inttoptr i64 %244 to ptr
  %246 = load ptr, ptr %245, align 8
  %247 = icmp ne ptr %246, null
  %248 = or i1 %243, %247
  %249 = add i64 %shadow.ptr.int38, 24
  %250 = inttoptr i64 %249 to ptr
  %251 = load ptr, ptr %250, align 8
  %252 = icmp ne ptr %251, null
  %253 = or i1 %248, %252
  %254 = add i64 %shadow.ptr.int38, 32
  %255 = inttoptr i64 %254 to ptr
  %256 = load ptr, ptr %255, align 8
  %257 = icmp ne ptr %256, null
  %258 = or i1 %253, %257
  %259 = add i64 %shadow.ptr.int38, 40
  %260 = inttoptr i64 %259 to ptr
  %261 = load ptr, ptr %260, align 8
  %262 = icmp ne ptr %261, null
  %263 = or i1 %258, %262
  %264 = add i64 %shadow.ptr.int38, 48
  %265 = inttoptr i64 %264 to ptr
  %266 = load ptr, ptr %265, align 8
  %267 = icmp ne ptr %266, null
  %268 = or i1 %263, %267
  %269 = add i64 %shadow.ptr.int38, 56
  %270 = inttoptr i64 %269 to ptr
  %271 = load ptr, ptr %270, align 8
  %272 = icmp ne ptr %271, null
  %273 = or i1 %268, %272
  br i1 %273, label %274, label %275, !prof !8

274:                                              ; preds = %238
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 1)
  br label %275

275:                                              ; preds = %238, %274
  store ptr @__tysan_v1_double_2a_o_0, ptr %shadow.ptr39, align 8
  %shadow.byte.1.offset42 = add i64 %shadow.ptr.int38, 8
  %shadow.byte.1.ptr43 = inttoptr i64 %shadow.byte.1.offset42 to ptr
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr43, align 8
  %shadow.byte.2.offset44 = add i64 %shadow.ptr.int38, 16
  %shadow.byte.2.ptr45 = inttoptr i64 %shadow.byte.2.offset44 to ptr
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr45, align 8
  %shadow.byte.3.offset46 = add i64 %shadow.ptr.int38, 24
  %shadow.byte.3.ptr47 = inttoptr i64 %shadow.byte.3.offset46 to ptr
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr47, align 8
  %shadow.byte.4.offset48 = add i64 %shadow.ptr.int38, 32
  %shadow.byte.4.ptr49 = inttoptr i64 %shadow.byte.4.offset48 to ptr
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr49, align 8
  %shadow.byte.5.offset50 = add i64 %shadow.ptr.int38, 40
  %shadow.byte.5.ptr51 = inttoptr i64 %shadow.byte.5.offset50 to ptr
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr51, align 8
  %shadow.byte.6.offset52 = add i64 %shadow.ptr.int38, 48
  %shadow.byte.6.ptr53 = inttoptr i64 %shadow.byte.6.offset52 to ptr
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr53, align 8
  %shadow.byte.7.offset54 = add i64 %shadow.ptr.int38, 56
  %shadow.byte.7.ptr55 = inttoptr i64 %shadow.byte.7.offset54 to ptr
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr55, align 8
  br label %277

276:                                              ; preds = %236
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 1)
  br label %277

277:                                              ; preds = %276, %275
  br label %323

278:                                              ; preds = %235
  %279 = add i64 %shadow.ptr.int38, 8
  %280 = inttoptr i64 %279 to ptr
  %281 = load ptr, ptr %280, align 8
  %282 = ptrtoint ptr %281 to i64
  %283 = icmp sge i64 %282, 0
  %284 = or i1 false, %283
  %285 = add i64 %shadow.ptr.int38, 16
  %286 = inttoptr i64 %285 to ptr
  %287 = load ptr, ptr %286, align 8
  %288 = ptrtoint ptr %287 to i64
  %289 = icmp sge i64 %288, 0
  %290 = or i1 %284, %289
  %291 = add i64 %shadow.ptr.int38, 24
  %292 = inttoptr i64 %291 to ptr
  %293 = load ptr, ptr %292, align 8
  %294 = ptrtoint ptr %293 to i64
  %295 = icmp sge i64 %294, 0
  %296 = or i1 %290, %295
  %297 = add i64 %shadow.ptr.int38, 32
  %298 = inttoptr i64 %297 to ptr
  %299 = load ptr, ptr %298, align 8
  %300 = ptrtoint ptr %299 to i64
  %301 = icmp sge i64 %300, 0
  %302 = or i1 %296, %301
  %303 = add i64 %shadow.ptr.int38, 40
  %304 = inttoptr i64 %303 to ptr
  %305 = load ptr, ptr %304, align 8
  %306 = ptrtoint ptr %305 to i64
  %307 = icmp sge i64 %306, 0
  %308 = or i1 %302, %307
  %309 = add i64 %shadow.ptr.int38, 48
  %310 = inttoptr i64 %309 to ptr
  %311 = load ptr, ptr %310, align 8
  %312 = ptrtoint ptr %311 to i64
  %313 = icmp sge i64 %312, 0
  %314 = or i1 %308, %313
  %315 = add i64 %shadow.ptr.int38, 56
  %316 = inttoptr i64 %315 to ptr
  %317 = load ptr, ptr %316, align 8
  %318 = ptrtoint ptr %317 to i64
  %319 = icmp sge i64 %318, 0
  %320 = or i1 %314, %319
  br i1 %320, label %321, label %322, !prof !8

321:                                              ; preds = %278
  call void @__tysan_check(ptr %ptr, i32 8, ptr @__tysan_v1_double_2a_o_0, i32 1)
  br label %322

322:                                              ; preds = %278, %321
  br label %323

323:                                              ; preds = %322, %277
  %324 = load ptr, ptr %ptr, align 8, !tbaa !15
  %app.ptr.int56 = ptrtoint ptr %324 to i64
  %app.ptr.masked57 = and i64 %app.ptr.int56, %app.mem.mask
  %app.ptr.shifted58 = shl i64 %app.ptr.masked57, 3
  %shadow.ptr.int59 = add i64 %app.ptr.shifted58, %shadow.base
  %shadow.ptr60 = inttoptr i64 %shadow.ptr.int59 to ptr
  %shadow.desc61 = load ptr, ptr %shadow.ptr60, align 8
  %bad.desc62 = icmp ne ptr %shadow.desc61, @__tysan_v1_double_o_0
  br i1 %bad.desc62, label %325, label %367, !prof !8

325:                                              ; preds = %323
  %326 = icmp eq ptr %shadow.desc61, null
  br i1 %326, label %327, label %365

327:                                              ; preds = %325
  %328 = add i64 %shadow.ptr.int59, 8
  %329 = inttoptr i64 %328 to ptr
  %330 = load ptr, ptr %329, align 8
  %331 = icmp ne ptr %330, null
  %332 = or i1 false, %331
  %333 = add i64 %shadow.ptr.int59, 16
  %334 = inttoptr i64 %333 to ptr
  %335 = load ptr, ptr %334, align 8
  %336 = icmp ne ptr %335, null
  %337 = or i1 %332, %336
  %338 = add i64 %shadow.ptr.int59, 24
  %339 = inttoptr i64 %338 to ptr
  %340 = load ptr, ptr %339, align 8
  %341 = icmp ne ptr %340, null
  %342 = or i1 %337, %341
  %343 = add i64 %shadow.ptr.int59, 32
  %344 = inttoptr i64 %343 to ptr
  %345 = load ptr, ptr %344, align 8
  %346 = icmp ne ptr %345, null
  %347 = or i1 %342, %346
  %348 = add i64 %shadow.ptr.int59, 40
  %349 = inttoptr i64 %348 to ptr
  %350 = load ptr, ptr %349, align 8
  %351 = icmp ne ptr %350, null
  %352 = or i1 %347, %351
  %353 = add i64 %shadow.ptr.int59, 48
  %354 = inttoptr i64 %353 to ptr
  %355 = load ptr, ptr %354, align 8
  %356 = icmp ne ptr %355, null
  %357 = or i1 %352, %356
  %358 = add i64 %shadow.ptr.int59, 56
  %359 = inttoptr i64 %358 to ptr
  %360 = load ptr, ptr %359, align 8
  %361 = icmp ne ptr %360, null
  %362 = or i1 %357, %361
  br i1 %362, label %363, label %364, !prof !8

363:                                              ; preds = %327
  call void @__tysan_check(ptr %324, i32 8, ptr @__tysan_v1_double_o_0, i32 1)
  br label %364

364:                                              ; preds = %327, %363
  store ptr @__tysan_v1_double_o_0, ptr %shadow.ptr60, align 8
  %shadow.byte.1.offset63 = add i64 %shadow.ptr.int59, 8
  %shadow.byte.1.ptr64 = inttoptr i64 %shadow.byte.1.offset63 to ptr
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr64, align 8
  %shadow.byte.2.offset65 = add i64 %shadow.ptr.int59, 16
  %shadow.byte.2.ptr66 = inttoptr i64 %shadow.byte.2.offset65 to ptr
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr66, align 8
  %shadow.byte.3.offset67 = add i64 %shadow.ptr.int59, 24
  %shadow.byte.3.ptr68 = inttoptr i64 %shadow.byte.3.offset67 to ptr
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr68, align 8
  %shadow.byte.4.offset69 = add i64 %shadow.ptr.int59, 32
  %shadow.byte.4.ptr70 = inttoptr i64 %shadow.byte.4.offset69 to ptr
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr70, align 8
  %shadow.byte.5.offset71 = add i64 %shadow.ptr.int59, 40
  %shadow.byte.5.ptr72 = inttoptr i64 %shadow.byte.5.offset71 to ptr
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr72, align 8
  %shadow.byte.6.offset73 = add i64 %shadow.ptr.int59, 48
  %shadow.byte.6.ptr74 = inttoptr i64 %shadow.byte.6.offset73 to ptr
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr74, align 8
  %shadow.byte.7.offset75 = add i64 %shadow.ptr.int59, 56
  %shadow.byte.7.ptr76 = inttoptr i64 %shadow.byte.7.offset75 to ptr
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr76, align 8
  br label %366

365:                                              ; preds = %325
  call void @__tysan_check(ptr %324, i32 8, ptr @__tysan_v1_double_o_0, i32 1)
  br label %366

366:                                              ; preds = %365, %364
  br label %412

367:                                              ; preds = %323
  %368 = add i64 %shadow.ptr.int59, 8
  %369 = inttoptr i64 %368 to ptr
  %370 = load ptr, ptr %369, align 8
  %371 = ptrtoint ptr %370 to i64
  %372 = icmp sge i64 %371, 0
  %373 = or i1 false, %372
  %374 = add i64 %shadow.ptr.int59, 16
  %375 = inttoptr i64 %374 to ptr
  %376 = load ptr, ptr %375, align 8
  %377 = ptrtoint ptr %376 to i64
  %378 = icmp sge i64 %377, 0
  %379 = or i1 %373, %378
  %380 = add i64 %shadow.ptr.int59, 24
  %381 = inttoptr i64 %380 to ptr
  %382 = load ptr, ptr %381, align 8
  %383 = ptrtoint ptr %382 to i64
  %384 = icmp sge i64 %383, 0
  %385 = or i1 %379, %384
  %386 = add i64 %shadow.ptr.int59, 32
  %387 = inttoptr i64 %386 to ptr
  %388 = load ptr, ptr %387, align 8
  %389 = ptrtoint ptr %388 to i64
  %390 = icmp sge i64 %389, 0
  %391 = or i1 %385, %390
  %392 = add i64 %shadow.ptr.int59, 40
  %393 = inttoptr i64 %392 to ptr
  %394 = load ptr, ptr %393, align 8
  %395 = ptrtoint ptr %394 to i64
  %396 = icmp sge i64 %395, 0
  %397 = or i1 %391, %396
  %398 = add i64 %shadow.ptr.int59, 48
  %399 = inttoptr i64 %398 to ptr
  %400 = load ptr, ptr %399, align 8
  %401 = ptrtoint ptr %400 to i64
  %402 = icmp sge i64 %401, 0
  %403 = or i1 %397, %402
  %404 = add i64 %shadow.ptr.int59, 56
  %405 = inttoptr i64 %404 to ptr
  %406 = load ptr, ptr %405, align 8
  %407 = ptrtoint ptr %406 to i64
  %408 = icmp sge i64 %407, 0
  %409 = or i1 %403, %408
  br i1 %409, label %410, label %411, !prof !8

410:                                              ; preds = %367
  call void @__tysan_check(ptr %324, i32 8, ptr @__tysan_v1_double_o_0, i32 1)
  br label %411

411:                                              ; preds = %367, %410
  br label %412

412:                                              ; preds = %411, %366
  %413 = load double, ptr %324, align 8, !tbaa !17
  %414 = fadd double %413, 2.000000e+00
  %app.ptr.int77 = ptrtoint ptr %324 to i64
  %app.ptr.masked78 = and i64 %app.ptr.int77, %app.mem.mask
  %app.ptr.shifted79 = shl i64 %app.ptr.masked78, 3
  %shadow.ptr.int80 = add i64 %app.ptr.shifted79, %shadow.base
  %shadow.ptr81 = inttoptr i64 %shadow.ptr.int80 to ptr
  %shadow.desc82 = load ptr, ptr %shadow.ptr81, align 8
  %bad.desc83 = icmp ne ptr %shadow.desc82, null
  br i1 %bad.desc83, label %415, label %457, !prof !8

415:                                              ; preds = %412
  %416 = icmp eq ptr %shadow.desc82, null
  br i1 %416, label %417, label %455

417:                                              ; preds = %415
  %418 = add i64 %shadow.ptr.int80, 8
  %419 = inttoptr i64 %418 to ptr
  %420 = load ptr, ptr %419, align 8
  %421 = icmp ne ptr %420, null
  %422 = or i1 false, %421
  %423 = add i64 %shadow.ptr.int80, 16
  %424 = inttoptr i64 %423 to ptr
  %425 = load ptr, ptr %424, align 8
  %426 = icmp ne ptr %425, null
  %427 = or i1 %422, %426
  %428 = add i64 %shadow.ptr.int80, 24
  %429 = inttoptr i64 %428 to ptr
  %430 = load ptr, ptr %429, align 8
  %431 = icmp ne ptr %430, null
  %432 = or i1 %427, %431
  %433 = add i64 %shadow.ptr.int80, 32
  %434 = inttoptr i64 %433 to ptr
  %435 = load ptr, ptr %434, align 8
  %436 = icmp ne ptr %435, null
  %437 = or i1 %432, %436
  %438 = add i64 %shadow.ptr.int80, 40
  %439 = inttoptr i64 %438 to ptr
  %440 = load ptr, ptr %439, align 8
  %441 = icmp ne ptr %440, null
  %442 = or i1 %437, %441
  %443 = add i64 %shadow.ptr.int80, 48
  %444 = inttoptr i64 %443 to ptr
  %445 = load ptr, ptr %444, align 8
  %446 = icmp ne ptr %445, null
  %447 = or i1 %442, %446
  %448 = add i64 %shadow.ptr.int80, 56
  %449 = inttoptr i64 %448 to ptr
  %450 = load ptr, ptr %449, align 8
  %451 = icmp ne ptr %450, null
  %452 = or i1 %447, %451
  br i1 %452, label %453, label %454, !prof !8

453:                                              ; preds = %417
  call void @__tysan_check(ptr %324, i32 8, ptr null, i32 2)
  br label %454

454:                                              ; preds = %417, %453
  store ptr null, ptr %shadow.ptr81, align 8
  %shadow.byte.1.offset84 = add i64 %shadow.ptr.int80, 8
  %shadow.byte.1.ptr85 = inttoptr i64 %shadow.byte.1.offset84 to ptr
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr85, align 8
  %shadow.byte.2.offset86 = add i64 %shadow.ptr.int80, 16
  %shadow.byte.2.ptr87 = inttoptr i64 %shadow.byte.2.offset86 to ptr
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr87, align 8
  %shadow.byte.3.offset88 = add i64 %shadow.ptr.int80, 24
  %shadow.byte.3.ptr89 = inttoptr i64 %shadow.byte.3.offset88 to ptr
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr89, align 8
  %shadow.byte.4.offset90 = add i64 %shadow.ptr.int80, 32
  %shadow.byte.4.ptr91 = inttoptr i64 %shadow.byte.4.offset90 to ptr
  store ptr inttoptr (i64 -4 to ptr), ptr %shadow.byte.4.ptr91, align 8
  %shadow.byte.5.offset92 = add i64 %shadow.ptr.int80, 40
  %shadow.byte.5.ptr93 = inttoptr i64 %shadow.byte.5.offset92 to ptr
  store ptr inttoptr (i64 -5 to ptr), ptr %shadow.byte.5.ptr93, align 8
  %shadow.byte.6.offset94 = add i64 %shadow.ptr.int80, 48
  %shadow.byte.6.ptr95 = inttoptr i64 %shadow.byte.6.offset94 to ptr
  store ptr inttoptr (i64 -6 to ptr), ptr %shadow.byte.6.ptr95, align 8
  %shadow.byte.7.offset96 = add i64 %shadow.ptr.int80, 56
  %shadow.byte.7.ptr97 = inttoptr i64 %shadow.byte.7.offset96 to ptr
  store ptr inttoptr (i64 -7 to ptr), ptr %shadow.byte.7.ptr97, align 8
  br label %456

455:                                              ; preds = %415
  call void @__tysan_check(ptr %324, i32 8, ptr null, i32 2)
  br label %456

456:                                              ; preds = %455, %454
  br label %502

457:                                              ; preds = %412
  %458 = add i64 %shadow.ptr.int80, 8
  %459 = inttoptr i64 %458 to ptr
  %460 = load ptr, ptr %459, align 8
  %461 = ptrtoint ptr %460 to i64
  %462 = icmp sge i64 %461, 0
  %463 = or i1 false, %462
  %464 = add i64 %shadow.ptr.int80, 16
  %465 = inttoptr i64 %464 to ptr
  %466 = load ptr, ptr %465, align 8
  %467 = ptrtoint ptr %466 to i64
  %468 = icmp sge i64 %467, 0
  %469 = or i1 %463, %468
  %470 = add i64 %shadow.ptr.int80, 24
  %471 = inttoptr i64 %470 to ptr
  %472 = load ptr, ptr %471, align 8
  %473 = ptrtoint ptr %472 to i64
  %474 = icmp sge i64 %473, 0
  %475 = or i1 %469, %474
  %476 = add i64 %shadow.ptr.int80, 32
  %477 = inttoptr i64 %476 to ptr
  %478 = load ptr, ptr %477, align 8
  %479 = ptrtoint ptr %478 to i64
  %480 = icmp sge i64 %479, 0
  %481 = or i1 %475, %480
  %482 = add i64 %shadow.ptr.int80, 40
  %483 = inttoptr i64 %482 to ptr
  %484 = load ptr, ptr %483, align 8
  %485 = ptrtoint ptr %484 to i64
  %486 = icmp sge i64 %485, 0
  %487 = or i1 %481, %486
  %488 = add i64 %shadow.ptr.int80, 48
  %489 = inttoptr i64 %488 to ptr
  %490 = load ptr, ptr %489, align 8
  %491 = ptrtoint ptr %490 to i64
  %492 = icmp sge i64 %491, 0
  %493 = or i1 %487, %492
  %494 = add i64 %shadow.ptr.int80, 56
  %495 = inttoptr i64 %494 to ptr
  %496 = load ptr, ptr %495, align 8
  %497 = ptrtoint ptr %496 to i64
  %498 = icmp sge i64 %497, 0
  %499 = or i1 %493, %498
  br i1 %499, label %500, label %501, !prof !8

500:                                              ; preds = %457
  call void @__tysan_check(ptr %324, i32 8, ptr null, i32 2)
  br label %501

501:                                              ; preds = %457, %500
  br label %502

502:                                              ; preds = %501, %456
  store double %414, ptr %324, align 8
  %app.ptr.int98 = ptrtoint ptr %result to i64
  %app.ptr.masked99 = and i64 %app.ptr.int98, %app.mem.mask
  %app.ptr.shifted100 = shl i64 %app.ptr.masked99, 3
  %shadow.ptr.int101 = add i64 %app.ptr.shifted100, %shadow.base
  %shadow.ptr102 = inttoptr i64 %shadow.ptr.int101 to ptr
  %shadow.desc103 = load ptr, ptr %shadow.ptr102, align 8
  %bad.desc104 = icmp ne ptr %shadow.desc103, null
  br i1 %bad.desc104, label %503, label %525, !prof !8

503:                                              ; preds = %502
  %504 = icmp eq ptr %shadow.desc103, null
  br i1 %504, label %505, label %523

505:                                              ; preds = %503
  %506 = add i64 %shadow.ptr.int101, 8
  %507 = inttoptr i64 %506 to ptr
  %508 = load ptr, ptr %507, align 8
  %509 = icmp ne ptr %508, null
  %510 = or i1 false, %509
  %511 = add i64 %shadow.ptr.int101, 16
  %512 = inttoptr i64 %511 to ptr
  %513 = load ptr, ptr %512, align 8
  %514 = icmp ne ptr %513, null
  %515 = or i1 %510, %514
  %516 = add i64 %shadow.ptr.int101, 24
  %517 = inttoptr i64 %516 to ptr
  %518 = load ptr, ptr %517, align 8
  %519 = icmp ne ptr %518, null
  %520 = or i1 %515, %519
  br i1 %520, label %521, label %522, !prof !8

521:                                              ; preds = %505
  call void @__tysan_check(ptr %result, i32 4, ptr null, i32 1)
  br label %522

522:                                              ; preds = %505, %521
  store ptr null, ptr %shadow.ptr102, align 8
  %shadow.byte.1.offset105 = add i64 %shadow.ptr.int101, 8
  %shadow.byte.1.ptr106 = inttoptr i64 %shadow.byte.1.offset105 to ptr
  store ptr inttoptr (i64 -1 to ptr), ptr %shadow.byte.1.ptr106, align 8
  %shadow.byte.2.offset107 = add i64 %shadow.ptr.int101, 16
  %shadow.byte.2.ptr108 = inttoptr i64 %shadow.byte.2.offset107 to ptr
  store ptr inttoptr (i64 -2 to ptr), ptr %shadow.byte.2.ptr108, align 8
  %shadow.byte.3.offset109 = add i64 %shadow.ptr.int101, 24
  %shadow.byte.3.ptr110 = inttoptr i64 %shadow.byte.3.offset109 to ptr
  store ptr inttoptr (i64 -3 to ptr), ptr %shadow.byte.3.ptr110, align 8
  br label %524

523:                                              ; preds = %503
  call void @__tysan_check(ptr %result, i32 4, ptr null, i32 1)
  br label %524

524:                                              ; preds = %523, %522
  br label %546

525:                                              ; preds = %502
  %526 = add i64 %shadow.ptr.int101, 8
  %527 = inttoptr i64 %526 to ptr
  %528 = load ptr, ptr %527, align 8
  %529 = ptrtoint ptr %528 to i64
  %530 = icmp sge i64 %529, 0
  %531 = or i1 false, %530
  %532 = add i64 %shadow.ptr.int101, 16
  %533 = inttoptr i64 %532 to ptr
  %534 = load ptr, ptr %533, align 8
  %535 = ptrtoint ptr %534 to i64
  %536 = icmp sge i64 %535, 0
  %537 = or i1 %531, %536
  %538 = add i64 %shadow.ptr.int101, 24
  %539 = inttoptr i64 %538 to ptr
  %540 = load ptr, ptr %539, align 8
  %541 = ptrtoint ptr %540 to i64
  %542 = icmp sge i64 %541, 0
  %543 = or i1 %537, %542
  br i1 %543, label %544, label %545, !prof !8

544:                                              ; preds = %525
  call void @__tysan_check(ptr %result, i32 4, ptr null, i32 1)
  br label %545

545:                                              ; preds = %525, %544
  br label %546

546:                                              ; preds = %545, %524
  %547 = load i32, ptr %result, align 4
  ret i32 %547
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

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{i64 -5436590890822487957, !"int"}
!6 = !{i64 6042226155786013045, !"long"}
!7 = !{i64 7516313782037075379, !"double*"}
!8 = !{!"branch_weights", i32 1, i32 100000}
!9 = !{!10, !10, i64 0}
!10 = !{!"int", !11, i64 0}
!11 = !{!"omnipotent byte", !12, i64 0}
!12 = !{!"Simple Spice TBAA"}
!13 = !{!14, !14, i64 0}
!14 = !{!"long", !11, i64 0}
!15 = !{!16, !16, i64 0}
!16 = !{!"double*", !11, i64 0}
!17 = !{!18, !18, i64 0}
!18 = !{!"double", !11, i64 0}
