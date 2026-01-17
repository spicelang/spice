; ModuleID = 'source.spice'
source_filename = "source.spice"

@__asan_shadow_memory_dynamic_address = external global i64
@__asan_option_detect_stack_use_after_return = external global i32
@___asan_gen_stack = private unnamed_addr constant [16 x i8] c"1 32 8 6 iPtr:4\00", align 1
@llvm.used = appending global [1 x ptr] [ptr @asan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 1, ptr @asan.module_ctor, ptr null }]

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_address uwtable
define dso_local noundef i32 @main() #0 !dbg !10 {
  %1 = load i64, ptr @__asan_shadow_memory_dynamic_address, align 8
  %result = alloca i32, align 4
  %asan_local_stack_base = alloca i64, align 8
  %2 = load i32, ptr @__asan_option_detect_stack_use_after_return, align 4
  %3 = icmp ne i32 %2, 0
  br i1 %3, label %4, label %6

4:                                                ; preds = %0
  %5 = call i64 @__asan_stack_malloc_0(i64 64)
  br label %6

6:                                                ; preds = %0, %4
  %7 = phi i64 [ 0, %0 ], [ %5, %4 ]
  %8 = inttoptr i64 %7 to ptr
  %9 = icmp eq i64 %7, 0
  br i1 %9, label %10, label %11

10:                                               ; preds = %6
  %MyAlloca = alloca i8, i64 64, align 32
  br label %11

11:                                               ; preds = %6, %10
  %12 = phi ptr [ %8, %6 ], [ %MyAlloca, %10 ]
  store ptr %12, ptr %asan_local_stack_base, align 8
  %13 = getelementptr i8, ptr %12, i64 32
  store i64 1102416563, ptr %12, align 8
  %14 = getelementptr i8, ptr %12, i64 8
  store i64 ptrtoint (ptr @___asan_gen_stack to i64), ptr %14, align 8
  %15 = getelementptr i8, ptr %12, i64 16
  store i64 ptrtoint (ptr @main to i64), ptr %15, align 8
  %16 = ptrtoint ptr %12 to i64
  %17 = lshr i64 %16, 3
  %18 = add i64 %17, %1
  %19 = add i64 %18, 0
  %20 = inttoptr i64 %19 to ptr
  store i64 -868082052615769615, ptr %20, align 1
  call void @llvm.lifetime.start.p0(ptr %result), !dbg !16
    #dbg_declare(ptr %result, !17, !DIExpression(), !16)
  store i32 0, ptr %result, align 4, !dbg !16
  %21 = call noundef ptr @_Z4sNewIiEPiv(), !dbg !18
  %22 = add i64 %18, 4, !dbg !18
  %23 = inttoptr i64 %22 to ptr, !dbg !18
  store i8 0, ptr %23, align 1, !dbg !18
  store ptr %21, ptr %13, align 8, !dbg !18
    #dbg_declare(ptr %asan_local_stack_base, !19, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 32), !21)
  %24 = load ptr, ptr %13, align 8, !dbg !22
  %25 = ptrtoint ptr %24 to i64, !dbg !23
  %26 = lshr i64 %25, 3, !dbg !23
  %27 = add i64 %26, %1, !dbg !23
  %28 = inttoptr i64 %27 to ptr, !dbg !23
  %29 = load i8, ptr %28, align 1, !dbg !23
  %30 = icmp ne i8 %29, 0, !dbg !23
  br i1 %30, label %31, label %37, !dbg !23, !prof !24

31:                                               ; preds = %11
  %32 = and i64 %25, 7, !dbg !23
  %33 = add i64 %32, 3, !dbg !23
  %34 = trunc i64 %33 to i8, !dbg !23
  %35 = icmp sge i8 %34, %29, !dbg !23
  br i1 %35, label %36, label %37, !dbg !23

36:                                               ; preds = %31
  call void @__asan_report_store4(i64 %25) #4, !dbg !23
  unreachable

37:                                               ; preds = %31, %11
  store i32 123, ptr %24, align 4, !dbg !23
  call void @_Z8sDeallocRPh(ptr noundef %13), !dbg !25
  %38 = load ptr, ptr %13, align 8, !dbg !27
  %39 = ptrtoint ptr %38 to i64, !dbg !28
  %40 = lshr i64 %39, 3, !dbg !28
  %41 = add i64 %40, %1, !dbg !28
  %42 = inttoptr i64 %41 to ptr, !dbg !28
  %43 = load i8, ptr %42, align 1, !dbg !28
  %44 = icmp ne i8 %43, 0, !dbg !28
  br i1 %44, label %45, label %51, !dbg !28, !prof !24

45:                                               ; preds = %37
  %46 = and i64 %39, 7, !dbg !28
  %47 = add i64 %46, 3, !dbg !28
  %48 = trunc i64 %47 to i8, !dbg !28
  %49 = icmp sge i8 %48, %43, !dbg !28
  br i1 %49, label %50, label %51, !dbg !28

50:                                               ; preds = %45
  call void @__asan_report_store4(i64 %39) #4, !dbg !28
  unreachable

51:                                               ; preds = %45, %37
  store i32 321, ptr %38, align 4, !dbg !28
  call void @_Z8sDeallocRPh(ptr %13), !dbg !29
  %52 = add i64 %18, 4, !dbg !29
  %53 = inttoptr i64 %52 to ptr, !dbg !29
  store i8 -8, ptr %53, align 1, !dbg !29
  %54 = load i32, ptr %result, align 4, !dbg !29
  store i64 1172321806, ptr %12, align 8, !dbg !29
  %55 = icmp ne i64 %7, 0, !dbg !29
  br i1 %55, label %56, label %62, !dbg !29

56:                                               ; preds = %51
  %57 = add i64 %18, 0, !dbg !29
  %58 = inttoptr i64 %57 to ptr, !dbg !29
  store i64 -723401728380766731, ptr %58, align 1, !dbg !29
  %59 = getelementptr i8, ptr %8, i64 56, !dbg !29
  %60 = load i64, ptr %59, align 8, !dbg !29
  %61 = inttoptr i64 %60 to ptr, !dbg !29
  store i8 0, ptr %61, align 1, !dbg !29
  br label %65, !dbg !29

62:                                               ; preds = %51
  %63 = add i64 %18, 0, !dbg !29
  %64 = inttoptr i64 %63 to ptr, !dbg !29
  store i64 0, ptr %64, align 1, !dbg !29
  br label %65, !dbg !29

65:                                               ; preds = %62, %56
  ret i32 %54, !dbg !29
}

; Function Attrs: nobuiltin nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0(ptr captures(none)) #1

declare ptr @_Z4sNewIiEPiv()

declare void @_Z8sDeallocRPh(ptr)

; Function Attrs: nobuiltin nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0(ptr captures(none)) #1

declare void @__asan_report_load_n(i64, i64)

declare void @__asan_loadN(i64, i64)

declare void @__asan_report_load1(i64)

declare void @__asan_load1(i64)

declare void @__asan_report_load2(i64)

declare void @__asan_load2(i64)

declare void @__asan_report_load4(i64)

declare void @__asan_load4(i64)

declare void @__asan_report_load8(i64)

declare void @__asan_load8(i64)

declare void @__asan_report_load16(i64)

declare void @__asan_load16(i64)

declare void @__asan_report_store_n(i64, i64)

declare void @__asan_storeN(i64, i64)

declare void @__asan_report_store1(i64)

declare void @__asan_store1(i64)

declare void @__asan_report_store2(i64)

declare void @__asan_store2(i64)

declare void @__asan_report_store4(i64)

declare void @__asan_store4(i64)

declare void @__asan_report_store8(i64)

declare void @__asan_store8(i64)

declare void @__asan_report_store16(i64)

declare void @__asan_store16(i64)

declare void @__asan_report_exp_load_n(i64, i64, i32)

declare void @__asan_exp_loadN(i64, i64, i32)

declare void @__asan_report_exp_load1(i64, i32)

declare void @__asan_exp_load1(i64, i32)

declare void @__asan_report_exp_load2(i64, i32)

declare void @__asan_exp_load2(i64, i32)

declare void @__asan_report_exp_load4(i64, i32)

declare void @__asan_exp_load4(i64, i32)

declare void @__asan_report_exp_load8(i64, i32)

declare void @__asan_exp_load8(i64, i32)

declare void @__asan_report_exp_load16(i64, i32)

declare void @__asan_exp_load16(i64, i32)

declare void @__asan_report_exp_store_n(i64, i64, i32)

declare void @__asan_exp_storeN(i64, i64, i32)

declare void @__asan_report_exp_store1(i64, i32)

declare void @__asan_exp_store1(i64, i32)

declare void @__asan_report_exp_store2(i64, i32)

declare void @__asan_exp_store2(i64, i32)

declare void @__asan_report_exp_store4(i64, i32)

declare void @__asan_exp_store4(i64, i32)

declare void @__asan_report_exp_store8(i64, i32)

declare void @__asan_exp_store8(i64, i32)

declare void @__asan_report_exp_store16(i64, i32)

declare void @__asan_exp_store16(i64, i32)

declare ptr @__asan_memmove(ptr, ptr, i64)

declare ptr @__asan_memcpy(ptr, ptr, i64)

declare ptr @__asan_memset(ptr, i32, i64)

declare void @__asan_handle_no_return()

declare void @__sanitizer_ptr_cmp(i64, i64)

declare void @__sanitizer_ptr_sub(i64, i64)

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.shared(ptr) #2

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.private(ptr) #2

declare i64 @__asan_stack_malloc_0(i64)

declare void @__asan_stack_free_0(i64, i64)

declare i64 @__asan_stack_malloc_1(i64)

declare void @__asan_stack_free_1(i64, i64)

declare i64 @__asan_stack_malloc_2(i64)

declare void @__asan_stack_free_2(i64, i64)

declare i64 @__asan_stack_malloc_3(i64)

declare void @__asan_stack_free_3(i64, i64)

declare i64 @__asan_stack_malloc_4(i64)

declare void @__asan_stack_free_4(i64, i64)

declare i64 @__asan_stack_malloc_5(i64)

declare void @__asan_stack_free_5(i64, i64)

declare i64 @__asan_stack_malloc_6(i64)

declare void @__asan_stack_free_6(i64, i64)

declare i64 @__asan_stack_malloc_7(i64)

declare void @__asan_stack_free_7(i64, i64)

declare i64 @__asan_stack_malloc_8(i64)

declare void @__asan_stack_free_8(i64, i64)

declare i64 @__asan_stack_malloc_9(i64)

declare void @__asan_stack_free_9(i64, i64)

declare i64 @__asan_stack_malloc_10(i64)

declare void @__asan_stack_free_10(i64, i64)

declare void @__asan_poison_stack_memory(i64, i64)

declare void @__asan_unpoison_stack_memory(i64, i64)

declare void @__asan_set_shadow_00(i64, i64)

declare void @__asan_set_shadow_01(i64, i64)

declare void @__asan_set_shadow_02(i64, i64)

declare void @__asan_set_shadow_03(i64, i64)

declare void @__asan_set_shadow_04(i64, i64)

declare void @__asan_set_shadow_05(i64, i64)

declare void @__asan_set_shadow_06(i64, i64)

declare void @__asan_set_shadow_07(i64, i64)

declare void @__asan_set_shadow_f1(i64, i64)

declare void @__asan_set_shadow_f2(i64, i64)

declare void @__asan_set_shadow_f3(i64, i64)

declare void @__asan_set_shadow_f5(i64, i64)

declare void @__asan_set_shadow_f8(i64, i64)

declare void @__asan_alloca_poison(i64, i64)

declare void @__asan_allocas_unpoison(i64, i64)

declare void @__asan_before_dynamic_init(i64)

declare void @__asan_after_dynamic_init()

declare void @__asan_register_globals(i64, i64)

declare void @__asan_unregister_globals(i64, i64)

declare void @__asan_register_image_globals(i64)

declare void @__asan_unregister_image_globals(i64)

declare void @__asan_register_elf_globals(i64, i64, i64)

declare void @__asan_unregister_elf_globals(i64, i64, i64)

declare void @__asan_init()

; Function Attrs: nounwind uwtable
define internal void @asan.module_ctor() #3 {
  call void @__asan_init()
  call void @__asan_version_mismatch_check_v8()
  ret void
}

declare void @__asan_version_mismatch_check_v8()

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_address uwtable }
attributes #1 = { nobuiltin nocallback nofree nosync nounwind willreturn }
attributes #2 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { nounwind uwtable "frame-pointer"="all" }
attributes #4 = { nomerge }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6}
!llvm.ident = !{!7}
!llvm.dbg.cu = !{!8}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{i32 4, !"nosanitize_address", i32 1}
!7 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!8 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !9, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)

!10 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !11, file: !11, line: 3, type: !12, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !8, retainedNodes: !15)

!12 = !DISubroutineType(types: !13)
!13 = !{!14}
!14 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!15 = !{}
!16 = !DILocation(line: 3, column: 1, scope: !10)
!17 = !DILocalVariable(name: "result", scope: !10, file: !11, line: 3, type: !14)
!18 = !DILocation(line: 4, column: 22, scope: !10)
!19 = !DILocalVariable(name: "iPtr", scope: !10, file: !11, line: 4, type: !20)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!21 = !DILocation(line: 4, column: 5, scope: !10)
!22 = !DILocation(line: 5, column: 6, scope: !10)
!23 = !DILocation(line: 5, column: 13, scope: !10)
!24 = !{!"branch_weights", i32 1, i32 1048575}
!25 = !DILocation(line: 7, column: 35, scope: !26)
!26 = distinct !DILexicalBlock(scope: !10, file: !11, line: 6, column: 5)
!27 = !DILocation(line: 9, column: 6, scope: !10)
!28 = !DILocation(line: 9, column: 13, scope: !10)
!29 = !DILocation(line: 10, column: 1, scope: !10)
