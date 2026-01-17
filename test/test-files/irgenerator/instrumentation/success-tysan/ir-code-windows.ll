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
define dso_local noundef i32 @main() #0 {
  %app.mem.mask = load i64, ptr @__tysan_app_memory_mask, align 8
  %shadow.base = load i64, ptr @__tysan_shadow_memory_address, align 8
  %result = alloca i32, align 4, !type !5
  call void @__tysan_instrument_mem_inst(ptr %result, ptr null, i64 4, i1 false)
  %l = alloca i64, align 8, !type !6
  call void @__tysan_instrument_mem_inst(ptr %l, ptr null, i64 8, i1 false)
  %ptr = alloca ptr, align 8, !type !7
  call void @__tysan_instrument_mem_inst(ptr %ptr, ptr null, i64 8, i1 false)
  call void @__tysan_instrument_with_shadow_update(ptr %result, ptr @__tysan_v1_int_o_0, i1 true, i64 4, i32 2)
  store i32 0, ptr %result, align 4, !tbaa !8
  call void @__tysan_instrument_with_shadow_update(ptr %l, ptr @__tysan_v1_long_o_0, i1 true, i64 8, i32 2)
  store i64 100, ptr %l, align 8, !tbaa !12
  call void @__tysan_instrument_with_shadow_update(ptr %ptr, ptr @__tysan_v1_double_2a_o_0, i1 true, i64 8, i32 2)
  store ptr %l, ptr %ptr, align 8, !tbaa !14
  call void @__tysan_instrument_with_shadow_update(ptr %ptr, ptr @__tysan_v1_double_2a_o_0, i1 true, i64 8, i32 1)
  %1 = load ptr, ptr %ptr, align 8, !tbaa !14
  call void @__tysan_instrument_with_shadow_update(ptr %1, ptr @__tysan_v1_double_o_0, i1 true, i64 8, i32 1)
  %2 = load double, ptr %1, align 8, !tbaa !16
  %3 = fadd double %2, 2.000000e+00
  call void @__tysan_instrument_with_shadow_update(ptr %1, ptr null, i1 true, i64 8, i32 2)
  store double %3, ptr %1, align 8
  call void @__tysan_instrument_with_shadow_update(ptr %result, ptr null, i1 true, i64 4, i32 1)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

declare void @__tysan_init()

; Function Attrs: nounwind uwtable
define internal void @tysan.module_ctor() #1 {
  call void @__tysan_init()
  ret void
}

; Function Attrs: nounwind
declare void @__tysan_check(ptr, i32, ptr, i32) #2

; Function Attrs: nounwind
declare void @__tysan_instrument_mem_inst(ptr, ptr, i64, i1) #2

; Function Attrs: nounwind
declare void @__tysan_instrument_with_shadow_update(ptr, ptr, i1, i64, i32) #2

; Function Attrs: nounwind
declare void @__tysan_set_shadow_type(ptr, ptr, i64) #2

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_type uwtable }
attributes #1 = { nounwind uwtable "frame-pointer"="all" }
attributes #2 = { nounwind }

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
!8 = !{!9, !9, i64 0}
!9 = !{!"int", !10, i64 0}
!10 = !{!"omnipotent byte", !11, i64 0}
!11 = !{!"Simple Spice TBAA"}
!12 = !{!13, !13, i64 0}
!13 = !{!"long", !10, i64 0}
!14 = !{!15, !15, i64 0}
!15 = !{!"double*", !10, i64 0}
!16 = !{!17, !17, i64 0}
!17 = !{!"double", !10, i64 0}
