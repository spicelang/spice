; ModuleID = 'source.spice'
source_filename = "source.spice"

@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @tysan.module_ctor, ptr null }]
@__tysan_v1_Simple_20Spice_20TBAA = linkonce_odr constant { i64, i64, [18 x i8] } { i64 2, i64 0, [18 x i8] c"Simple Spice TBAA\00" }
@__tysan_v1_omnipotent_20byte = linkonce_odr constant { i64, i64, ptr, i64, [16 x i8] } { i64 2, i64 1, ptr @__tysan_v1_Simple_20Spice_20TBAA, i64 0, [16 x i8] c"omnipotent byte\00" }
@__tysan_v1_long = linkonce_odr constant { i64, i64, ptr, i64, [5 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [5 x i8] c"long\00" }
@__tysan_v1_long_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_long, ptr @__tysan_v1_long, i64 0 }
@__tysan_v1_double_2a = linkonce_odr constant { i64, i64, ptr, i64, [8 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [8 x i8] c"double*\00" }
@__tysan_v1_double_2a_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_double_2a, ptr @__tysan_v1_double_2a, i64 0 }
@__tysan_v1_double = linkonce_odr constant { i64, i64, ptr, i64, [7 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [7 x i8] c"double\00" }
@__tysan_v1_double_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_double, ptr @__tysan_v1_double, i64 0 }
@llvm.used = appending global [9 x ptr] [ptr @tysan.module_ctor, ptr @__tysan_v1_Simple_20Spice_20TBAA, ptr @__tysan_v1_omnipotent_20byte, ptr @__tysan_v1_long, ptr @__tysan_v1_long_o_0, ptr @__tysan_v1_double_2a, ptr @__tysan_v1_double_2a_o_0, ptr @__tysan_v1_double, ptr @__tysan_v1_double_o_0], section "llvm.metadata"
@__tysan_shadow_memory_address = external global i64
@__tysan_app_memory_mask = external global i64

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_type uwtable
define dso_local noundef i32 @main() #0 !dbg !9 {
  %app.mem.mask = load i64, ptr @__tysan_app_memory_mask, align 8
  %shadow.base = load i64, ptr @__tysan_shadow_memory_address, align 8
  %l = alloca i64, align 8, !type !15
  call void @__tysan_instrument_mem_inst(ptr %l, ptr null, i64 8, i1 false)
  %ptr = alloca ptr, align 8, !dbg !16, !type !17
    #dbg_declare(ptr %l, !18, !DIExpression(), !16)
  call void @__tysan_instrument_mem_inst(ptr %ptr, ptr null, i64 8, i1 false), !dbg !16
  call void @__tysan_instrument_with_shadow_update(ptr %l, ptr @__tysan_v1_long_o_0, i1 true, i64 8, i32 2), !dbg !16
  store i64 100, ptr %l, align 8, !dbg !16, !tbaa !20
    #dbg_declare(ptr %ptr, !24, !DIExpression(), !28)
  call void @__tysan_instrument_with_shadow_update(ptr %ptr, ptr @__tysan_v1_double_2a_o_0, i1 true, i64 8, i32 2), !dbg !28
  store ptr %l, ptr %ptr, align 8, !dbg !28, !tbaa !29
  call void @__tysan_instrument_with_shadow_update(ptr %ptr, ptr null, i1 true, i64 8, i32 1), !dbg !31
  %1 = load ptr, ptr %ptr, align 8, !dbg !31
  call void @__tysan_instrument_with_shadow_update(ptr %1, ptr @__tysan_v1_double_o_0, i1 true, i64 8, i32 1), !dbg !31
  %2 = load double, ptr %1, align 8, !dbg !31, !tbaa !32
  %3 = fadd double %2, 2.000000e+00, !dbg !31
  call void @__tysan_instrument_with_shadow_update(ptr %1, ptr null, i1 true, i64 8, i32 2), !dbg !31
  store double %3, ptr %1, align 8, !dbg !31
  ret i32 0, !dbg !34
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

!9 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !10, file: !10, line: 3, type: !11, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7, retainedNodes: !14)

!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !{i64 6042226155786013045, !"long"}
!16 = !DILocation(line: 4, column: 14, scope: !9)
!17 = !{i64 7516313782037075379, !"double*"}
!18 = !DILocalVariable(name: "l", scope: !9, file: !10, line: 4, type: !19)
!19 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!20 = !{!21, !21, i64 0}
!21 = !{!"long", !22, i64 0}
!22 = !{!"omnipotent byte", !23, i64 0}
!23 = !{!"Simple Spice TBAA"}
!24 = !DILocalVariable(name: "ptr", scope: !25, file: !10, line: 6, type: !26)
!25 = distinct !DILexicalBlock(scope: !9, file: !10, line: 5, column: 5)
!26 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !27, size: 64)
!27 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!28 = !DILocation(line: 6, column: 37, scope: !25)
!29 = !{!30, !30, i64 0}
!30 = !{!"double*", !22, i64 0}
!31 = !DILocation(line: 7, column: 9, scope: !25)
!32 = !{!33, !33, i64 0}
!33 = !{!"double", !22, i64 0}
!34 = !DILocation(line: 9, column: 1, scope: !9)
